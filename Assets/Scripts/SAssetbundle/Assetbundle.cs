using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;
using UnityEngine;
#if UNITY_EDITOR
using UnityEditor;

#endif


namespace SAssetbundle
{
    public class BundleInfo
    {
        public string Name;
        public uint CRC;
        public string hash;
        public long Size;
    }

    public class BundleManifest
    {
        //todo optimise this slow way ,use hash name

        [JsonProperty] private List<BundleInfo> Bundles = new List<BundleInfo>();

        [JsonProperty]
        private Dictionary<string, List<string>> BundleAssetsMap = new Dictionary<string, List<string>>();

        [JsonProperty]
        private Dictionary<string, List<string>> AssetDependencyBundles = new Dictionary<string, List<string>>();

        [JsonProperty] private List<string> SceneAssetNames = new List<string>();

        /// <summary>
        /// asset to bundle map.
        /// </summary>
        private Dictionary<string, string> AssetBundleMap = new Dictionary<string, string>();

        public static string ManifestName = "main.txt";
        public static string ManifestBundleName = "manifest.ab";

        public static BundleManifest LoadFromDLCPath()
        {
            AssetBundle.UnloadAllAssetBundles(true);
            var path = Path.Combine(AssetBundleManager.Instance.GetDLCPath(), ManifestBundleName);
            if (File.Exists(path))
            {
                var assetBundle = AssetBundle.LoadFromFile(path);
                var jsonFile = assetBundle.LoadAsset<TextAsset>(ManifestName);
                assetBundle.Unload(false);
                return LoadFromJson(jsonFile.text);
            }

            return null;
        }

        public static BundleManifest LoadFromJson(string json)
        {
            var bundleManifest = JsonConvert.DeserializeObject<BundleManifest>(json);
            bundleManifest.InitAssetBundleMap();
            return bundleManifest;
        }


        public string GetScenePath(string sceneName)
        {
            var sceneNameWithExt = sceneName;
            if (!sceneNameWithExt.EndsWith(".unity"))
            {
                sceneNameWithExt = sceneName + ".unity";
            }

            foreach (var scene in SceneAssetNames)
            {
                if (scene.Contains(sceneNameWithExt))
                {
                    return scene;
                }
            }

            return string.Empty;
        }

        public void AddBundleInfo(string name, uint crc = 0, string hash = "", long size = 0)
        {
            BundleInfo bundleInfo = new BundleInfo();
            bundleInfo.Name = name;
            bundleInfo.CRC = crc;
            bundleInfo.hash = hash;
            bundleInfo.Size = size;
            Bundles.Add(bundleInfo);
        }

        public void AddBundleAsset(string bundleName, List<string> assets,bool isSceneBundle=false)
        {
            foreach (var asset in assets)
            {
                AddBundleAsset(bundleName, asset,isSceneBundle);
            }

            if (isSceneBundle)
            {
                AddScenes(assets);
            }
        }

        private void AddBundleAsset(string bundleName, string assetName,bool isSceneBundle)
        {
            assetName = assetName.ToLower();
            if (BundleAssetsMap.TryGetValue(bundleName, out List<string> assets))
            {
                if (!assets.Contains(assetName))
                {
                    assets.Add(assetName);
                }
                else
                {
                    Debug.LogError("duplicated asset Name: " + assetName);
                }
            }
            else
            {
                assets = new List<string> {assetName};
                BundleAssetsMap.Add(bundleName, assets);
            }
        }

        private void AddScenes(List<string> scenes)
        {
            foreach (var scene in scenes)
            {
                AddScene(scene);
            }
        }

        private void AddScene(string scene)
        {
            scene = scene.ToLower();
            SceneAssetNames.Add(scene);
        }


        private void InitAssetBundleMap()
        {
            AssetBundleMap.Clear();

            foreach (var assetMap in BundleAssetsMap)
            {
                var bundleName = assetMap.Key;
                var assets = assetMap.Value;
                foreach (var asset in assets)
                {
                    AssetBundleMap.Add(asset, bundleName);
                }
            }
        }

        public bool IsNeedLoadDependencyBundle(string assetName)
        {
            return AssetDependencyBundles.ContainsKey(assetName);
        }


        public List<string> GetDependencyBundles(string assetName)
        {
            if (IsNeedLoadDependencyBundle(assetName))
            {
                return AssetDependencyBundles[assetName];
            }

            return null;
        }



        private List<string> GetBundleAssets(string abName)
        {
            BundleAssetsMap.TryGetValue(abName, out List<string> assets);
            return assets;
        }

        public List<BundleInfo> GetBundleInfos()
        {
            return Bundles;
        }

        public string GetBundleName(string assetName)
        {
            if (AssetBundleMap.TryGetValue(assetName, out string bundleName))
            {
                return bundleName;
            }

            return String.Empty;
        }

        public bool IsScene(string assetPath)
        {
            return SceneAssetNames.Contains(assetPath);
        }


#if UNITY_EDITOR

        private List<string> GetAllAssets()
        {
            List<string> assets = new List<string>();
            foreach (var bundle in Bundles)
            {
                var bundleAssets = GetBundleAssets(bundle.Name);
                if (bundleAssets != null)
                {
                    assets.AddRange(bundleAssets);
                }
            }

            return assets;
        }
        
        public void GenerateDependency()
        {
            AssetDependencyBundles.Clear();
            var allAssets = GetAllAssets();

            foreach (var asset in allAssets)
            {
                List<string> bundles = new List<string>();
                var dependencies = AssetDatabase.GetDependencies(asset);
                foreach (var dependency in dependencies)
                {
                    if (!dependency.Equals(asset, StringComparison.OrdinalIgnoreCase))
                    {
                        var bundle = AssetDatabase.GetImplicitAssetBundleName(dependency);
                        var variantName = AssetDatabase.GetImplicitAssetBundleVariantName(dependency);
                        var bundleName = string.IsNullOrWhiteSpace(variantName) ? bundle : bundle + "." + variantName;
                        if (!string.IsNullOrEmpty(bundleName) && !bundles.Contains(bundleName))
                        {
                            bundles.Add(bundleName);
                        }
                    }
                }

                if (bundles.Count > 0)
                {
                    AssetDependencyBundles.Add(asset, bundles);
                }
            }
        }
#endif
    }
}