using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Base;
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
        private Dictionary<string, List<string>> AssetDependencies = new Dictionary<string, List<string>>();

        [JsonProperty] private Dictionary<string, List<string>> SceneBundleMap = new Dictionary<string, List<string>>();

        
        
        
        public static BundleManifest Load()
        {
            AssetBundle.UnloadAllAssetBundles(true);
            var assetBundle = AssetBundle.LoadFromFile(Path.Combine(Application.streamingAssetsPath, "manifest.ab"));
            var jsonFile = assetBundle.LoadAsset<TextAsset>("main.txt");
            Debug.Log(jsonFile.text);
            var bundleManifest = JsonConvert.DeserializeObject<BundleManifest>(jsonFile.text);
            return bundleManifest;
        }


        public void AddBundleInfo(string name, uint crc = 0, string hash="",long size = 0)
        {
            BundleInfo bundleInfo = new BundleInfo();
            bundleInfo.Name = name;
            bundleInfo.CRC = crc;
            bundleInfo.hash = hash;
            bundleInfo.Size = size;
            Bundles.Add(bundleInfo);
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


        public void AddBundleScene(string sceneBundleName, string scene)
        {
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


    public class AssetBundleManager : Singleton<AssetBundleManager>
    {
        private BundleManifest _bundleManifest;

        public void LoadManifest()
        {
            _bundleManifest = BundleManifest.Load();
        }

        public string GetDLCPath()
        {
            return Application.streamingAssetsPath;
        }
    }
}