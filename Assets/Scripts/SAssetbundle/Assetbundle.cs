using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using Base;
using Newtonsoft.Json;
using UnityEngine;
using UnityEngine.Networking;
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
        private Dictionary<string, List<string>> AssetDependencies = new Dictionary<string, List<string>>();

        [JsonProperty] private Dictionary<string, List<string>> SceneBundleMap = new Dictionary<string, List<string>>();

        public static string ManifestName = "main.txt";
        public static string ManifestBundleName = "manifest.ab";
        private Dictionary<string, string> AssetBundleMap = new Dictionary<string, string>();

        private Dictionary<string, string> SceneAssetBundleMap = new Dictionary<string, string>();

        public static BundleManifest Load()
        {
            AssetBundle.UnloadAllAssetBundles(true);
            var path = Path.Combine(AssetBundleManager.Instance.GetDLCPath(),ManifestBundleName );
            if (File.Exists(path))
            {
                var assetBundle = AssetBundle.LoadFromFile(path);
                var jsonFile = assetBundle.LoadAsset<TextAsset>(ManifestName);
//                Debug.Log(jsonFile.text);
                assetBundle.Unload(false);
                return LoadFromJson(jsonFile.text);
            }
            return null;
        }

        public static BundleManifest LoadFromJson(string json)
        {
            var bundleManifest = JsonConvert.DeserializeObject<BundleManifest>(json);
            bundleManifest.InitAssetBundleMap();
            bundleManifest.InitSceneAssetBundleMap();
            return bundleManifest;
        }

        public List<BundleInfo> GetBundleInfos()
        {
            return Bundles;
        }

        public string GetScenePath(string sceneName)
        {

            var sceneFullName = sceneName + ".unity";
            foreach (var scenePaths in SceneBundleMap.Values)
            {
                foreach (var scenePath in scenePaths)
                {
                    if (scenePath.Contains(sceneFullName))
                    {
                        return scenePath;
                    }
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

        private void InitSceneAssetBundleMap()
        {
            SceneAssetBundleMap.Clear();
            foreach (var sceneMap in SceneBundleMap)
            {
                var bundleName = sceneMap.Key;
                var scenesPath = sceneMap.Value;

                foreach (var scenePath in scenesPath)
                {
                    SceneAssetBundleMap.Add(scenePath, bundleName);
                }
            }
        }

        public string GetBundleName(string assetName)
        {
            if (AssetBundleMap.TryGetValue(assetName, out string bundleName))
            {
                return bundleName;
            }

            if (SceneAssetBundleMap.TryGetValue(assetName,out string sceneBundleName))
            {
                return sceneBundleName;
            }
            
            return String.Empty;
        }

        public bool IsNeedLoadDependencyBundle(string assetName)
        {
            return AssetDependencies.ContainsKey(assetName);
        }


        public List<string> GetDependencyAssets(string assetName)
        {
            if (IsNeedLoadDependencyBundle(assetName))
            {
                return AssetDependencies[assetName];
            }

            return null;
        }

        public List<string> GetAllAssets()
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

        private List<string> GetBundleAssets(string abName)
        {
            BundleAssetsMap.TryGetValue(abName, out List<string> assets);
            return assets;
        }

        private void AddBundleAsset(string bundleName, string assetName)
        {
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

        public void AddBundleScene(string sceneBundleName, List<string> scenes)
        {
            foreach (var scene in scenes)
            {
                AddBundleScene(sceneBundleName, scene);
            }
        }


        private void AddBundleScene(string sceneBundleName, string scene)
        {
            scene = scene.ToLower();
            if (SceneBundleMap.TryGetValue(sceneBundleName, out List<string> sceneAssets))
            {
                if (!sceneAssets.Contains(scene))
                {
                    sceneAssets.Add(scene);
                }
                else
                {
                    Debug.LogError("duplicated scene Name: " + scene);
                }
            }
            else
            {
                sceneAssets = new List<string> {scene};
                SceneBundleMap.Add(sceneBundleName, sceneAssets);
            }
        }

        public void AddBundleAsset(string bundleName, List<string> assets)
        {
            foreach (var asset in assets)
            {
                AddBundleAsset(bundleName, asset);
            }
        }


        public bool IsAssetInBundle(string asset)
        {
            var allAssets = GetAllAssets();
            if (allAssets.Contains(asset.ToLower()))
            {
                return true;
            }

            return false;
        }


        public bool IsScene(string assetPath)
        {
            foreach (var sceneMap in SceneAssetBundleMap)
            {
                if (sceneMap.Key.Equals(assetPath))
                {
                    return true;
                }
            }

            return false;
        }


#if UNITY_EDITOR
        public void GenerateDependency()
        {
            AssetDependencies.Clear();
            var allAssets = GetAllAssets();
            foreach (var asset in allAssets)
            {
                var assets = new List<string>();
                var dependencies = AssetDatabase.GetDependencies(asset);
                foreach (var dependency in dependencies)
                {
                    var dep = dependency.ToLower();
                    if (dep != asset && IsAssetInBundle(dep))
                    {
                        assets.Add(dep);
                    }
                }

                if (assets != null && assets.Count > 0)
                {
                    AssetDependencies.Add(asset, assets);
                }
            }
        }
#endif
    }
}