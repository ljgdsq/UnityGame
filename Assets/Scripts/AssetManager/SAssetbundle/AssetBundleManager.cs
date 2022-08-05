using System.Collections;
using System.Collections.Generic;
using System.IO;
using AssetManager;
using Base;
using TMPro;
using UnityEngine;

namespace SAssetbundle
{
    public class AssetBundleManager :Singleton<AssetBundleManager>
    {

        public BundleManifest BundleManifest { get; private set; }
//        private AssetBundleLoader _assetBundleLoader;
        private AssetBundleLoadCache _assetBundleCache=new AssetBundleLoadCache();

        public override void Init()
        {
            LoadManifest();
        }
        
        public void LoadManifest()
        {
            BundleManifest = BundleManifest.Load();
        }

        public string GetDLCPath()
        {
            return Path.Combine(GameApplication.GetWriteablePath(),"dlc/");
        }

//        public T LoadAsset<T>(string assetPath) where T : Object
//        {
//            return _assetBundleLoader.LoadAsset<T>(assetPath);
//        }
//
//        public AssetLoadRequest LoadAssetAsync(string assetPath)
//        {
//            var req = new AssetLoadRequest(assetPath);
//            _assetBundleLoader.LoadAssetAsync(req);
//            return req;
//        }

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

        public List<string> GetDependencyAssets(string assetName)
        {
            return BundleManifest.GetDependencyAssets(assetName);
        }


        public IEnumerator LoadAssetInBundle(AssetObject assetObject,bool cacheBundle=true)
        {
            var assetName = assetObject.path;
            var bundleName = GetAssetBundleName(assetName);

            var cachedBundle = _assetBundleCache.GetCachedBundle(bundleName);
            if (cachedBundle==null)
            {
                var request = AssetBundle.LoadFromFileAsync(Path.Combine( GetDLCPath(),bundleName));
                yield return request;
                cachedBundle = request.assetBundle;
            }
           
            if (IsNeedDependencyBundle(assetName))
            {
                var dependencyAssets = GetDependencyAssets(assetName);

                foreach (var dependencyAsset in dependencyAssets)
                {
                    yield return LoadAssetInBundle(dependencyAsset);
                }
            }

            if (cachedBundle)
            {
                var assetReq=cachedBundle.LoadAssetAsync(assetName);
                yield return assetReq;
                assetObject.asset = assetReq.asset;
                assetObject.isdone = true;
                if (!cacheBundle)
                {
                    cachedBundle.Unload(false);
                    Object.Destroy(cachedBundle);
                }
            }
            else
            {
                Debug.LogError("can not load asset :"+assetName);
            }
            
        }

        public IEnumerator LoadSceneInBundle(AssetObject assetObject,bool cacheBundle=true)
        {
            var assetName = assetObject.path;
            var bundleName = GetAssetBundleName(assetName);

            var cachedBundle = _assetBundleCache.GetCachedBundle(bundleName);
            if (cachedBundle==null)
            {
                var request = AssetBundle.LoadFromFileAsync(Path.Combine( GetDLCPath(),bundleName));
                yield return request;
                cachedBundle = request.assetBundle;
            }
           
            if (IsNeedDependencyBundle(assetName))
            {
                var dependencyAssets = GetDependencyAssets(assetName);

                foreach (var dependencyAsset in dependencyAssets)
                {
                    yield return LoadAssetInBundle(dependencyAsset);
                }
            }

            if (cachedBundle)
            {
//                var assetReq=cachedBundle.LoadAssetAsync(assetName);
//                yield return assetReq;
                assetObject.asset = null;
                assetObject.isdone = true;
                if (!cacheBundle)
                {
                    cachedBundle.Unload(false);
                    Object.Destroy(cachedBundle);
                }
            }
            else
            {
                Debug.LogError("can not load asset :"+assetName);
            }
            
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
        
        private IEnumerator LoadAssetInBundle(string assetName)
        {
            var bundleName = GetAssetBundleName(assetName);
            
            var cachedBundle = _assetBundleCache.GetCachedBundle(bundleName);
            if (cachedBundle == null)
            {

                var request = AssetBundle.LoadFromFileAsync(Path.Combine( GetDLCPath(),bundleName));
                yield return request;
                cachedBundle = request.assetBundle;
            }


            if (IsNeedDependencyBundle(assetName))
            {
                var dependencyAssets = GetDependencyAssets(assetName);

                foreach (var dependencyAsset in dependencyAssets)
                {
                    yield return LoadAssetInBundle(dependencyAsset);
                }
            }

            if (cachedBundle)
            {
                var assetReq=cachedBundle.LoadAssetAsync(assetName);
                yield return assetReq;
            }
            else
            {
                Debug.LogError("can not load asset :"+assetName);
            }
        }
        
    }
}