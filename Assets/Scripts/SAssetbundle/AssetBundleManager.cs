using System.Collections;
using System.Collections.Generic;
using Base;
using TMPro;
using UnityEngine;

namespace SAssetbundle
{
    public class AssetBundleManager : MonoSingleton<AssetBundleManager>
    {
        
   
        private BundleManifest _bundleManifest;
        private AssetBundleLoader _assetBundleLoader;

        public void LoadManifest()
        {
            _bundleManifest = BundleManifest.Load();
        }

        public string GetDLCPath()
        {
            return Application.streamingAssetsPath;
        }

        public T LoadAsset<T>(string assetPath) where T : Object
        {
            return _assetBundleLoader.LoadAsset<T>(assetPath);
        }

        public AssetLoadRequest LoadAssetAsync(string assetPath)
        {
            var req = new AssetLoadRequest(assetPath);
            _assetBundleLoader.LoadAssetAsync(req);
            return req;
        }

        public string GetAssetBundleName(string assetName)
        {
            return _bundleManifest.GetBundleName(assetName);
        }

        public bool IsNeedDependencyBundle(string assetName)
        {
            return _bundleManifest.IsNeedLoadDependencyBundle(assetName);
        }

        public List<string> GetDependencyAssets(string assetName)
        {
            return _bundleManifest.GetDependencyAssets(assetName);
        }


        public IEnumerator LoadAssetInBundle(AssetObject assetObject,bool cacheBundle=true)
        {
            var assetName = assetObject.path;

            var bundleName = GetAssetBundleName(assetName);
            var request = AssetBundle.LoadFromFileAsync(bundleName);
            yield return request;

            if (IsNeedDependencyBundle(assetName))
            {
                var dependencyAssets = GetDependencyAssets(assetName);

                foreach (var dependencyAsset in dependencyAssets)
                {
                    yield return LoadAssetInBundle(dependencyAsset);
                }
            }

            if (request.assetBundle)
            {
                var assetReq=request.assetBundle.LoadAssetAsync(assetName);
                yield return assetReq;
                assetObject.asset = assetReq.asset;
                if (!cacheBundle)
                {
                    request.assetBundle.Unload(false);
                    Destroy(request.assetBundle);
                }
            }
            else
            {
                Debug.LogError("can not load asset :"+assetName);
            }
            
        }

        private IEnumerator LoadAssetInBundle(string assetName)
        {
            var bundleName = GetAssetBundleName(assetName);
            var request = AssetBundle.LoadFromFileAsync(bundleName);
            yield return request;

            if (IsNeedDependencyBundle(assetName))
            {
                var dependencyAssets = GetDependencyAssets(assetName);

                foreach (var dependencyAsset in dependencyAssets)
                {
                    yield return LoadAssetInBundle(dependencyAsset);
                }
            }

            if (request.assetBundle)
            {
                var assetReq=request.assetBundle.LoadAssetAsync(assetName);
                yield return assetReq;
                
            }
            else
            {
                Debug.LogError("can not load asset :"+assetName);
            }
        }
        
        private void Update()
        {
            _assetBundleLoader.Update();
        }
    }
}