using System.Collections;
using System.Collections.Generic;
using System.IO;
using Base;
using UnityEngine;

namespace SAssetbundle
{
    public class AssetBundleManager : Singleton<AssetBundleManager>
    {
        public BundleManifest BundleManifest { get; private set; }
        private readonly AssetBundleLoadCache _assetBundleCache = new AssetBundleLoadCache();

        public override void Init()
        {
            LoadManifest();
        }

        public void LoadManifest()
        {
            BundleManifest = BundleManifest.LoadFromDLCPath();
        }

        public static string GetDLCPath()
        {
            return Path.Combine(GameApplication.GetWriteablePath(), "dlc/");
        }

        public string GetScenePath(string sceneName)
        {
            return BundleManifest.GetScenePath(sceneName);
        }

        public string GetAssetBundleName(string assetName)
        {
            return BundleManifest.GetBundleName(assetName);
        }

        public bool IsNeedDependencyBundle(string assetName)
        {
            return BundleManifest.IsNeedLoadDependencyBundle(assetName);
        }

        public List<string> GetDependencyBundles(string assetName)
        {
            return BundleManifest.GetDependencyBundles(assetName);
        }


        public IEnumerator LoadAssetInBundle(AssetObject assetObject, bool cacheBundle = true, bool isScene = false)
        {
            var assetName = assetObject.path;
            var bundleName = GetAssetBundleName(assetName);

            var loadedBundle = _assetBundleCache.GetCachedBundle(bundleName);
            if (loadedBundle == null)
            {
                var request = AssetBundle.LoadFromFileAsync(Path.Combine(GetDLCPath(), bundleName));
                yield return request;
                loadedBundle = request.assetBundle;
            }


            List<AssetBundle> depsBundle = new List<AssetBundle>();
            var dependencyBundles = GetDependencyBundles(assetName);
            if (dependencyBundles != null && dependencyBundles.Count > 0)
            {
                foreach (var dependencyBundle in dependencyBundles)
                {
                    if (_assetBundleCache.GetCachedBundle(dependencyBundle) == null)
                    {
                        var request = AssetBundle.LoadFromFileAsync(Path.Combine(GetDLCPath(), dependencyBundle));
                        yield return request;
                        depsBundle.Add(request.assetBundle);
                    }
                }
            }

            if (loadedBundle)
            {
                if (isScene)
                {
                    assetObject.isdone = true;
                }
                else
                {
                    var assetReq = loadedBundle.LoadAssetAsync(assetName);
                    yield return assetReq;
                    assetObject.asset = assetReq.asset;
                    assetObject.isdone = true;
                }

                if (!cacheBundle)
                {
                    loadedBundle.Unload(false);
                    Object.Destroy(loadedBundle);
                    foreach (var assetBundle in depsBundle)
                    {
                        assetBundle.Unload(false);
                        Object.Destroy(assetBundle);
                    }
                }
            }
            else
            {
                Debug.LogError("can not load asset :" + assetName);
            }
        }

        public IEnumerator LoadSceneInBundle(AssetObject assetObject, bool cacheBundle = true)
        {
            yield return LoadAssetInBundle(assetObject, cacheBundle,true);
        }


        public bool IsScene(string assetName)
        {
            return BundleManifest.IsScene(assetName);
        }

        public bool IsAssetInBundle(string assetName)
        {
            var bundleName = GetAssetBundleName(assetName);
            if (string.IsNullOrEmpty(bundleName))
            {
                return false;
            }

            return true;
        }

        public void DeleteBundle(string bundleName)
        {
            var path = Path.Combine(GetDLCPath(), bundleName);
            if (File.Exists(path))
            {
                File.Delete(path);
            }
        }
    }
}