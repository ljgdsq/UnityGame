using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class UnityCache
{
    IEnumerator DownloadAndCacheAB(string uri, string manifest)
    {

        AssetBundle manifestBundle=AssetBundle.LoadFromFile(manifest);

        AssetBundleManifest assetBundleManifest = manifestBundle.LoadAsset<AssetBundleManifest>("AssetBundleManifest");


        string today = DateTime.Today.ToLongDateString();
        Cache newCache = Caching.AddCache(today);

        if (newCache.valid)
        {
            Caching.currentCacheForWriting = newCache;
        }

        Hash128 bundleHash = assetBundleManifest.GetAssetBundleHash("MyBundle");

        UnityWebRequest request = UnityWebRequestAssetBundle.GetAssetBundle(uri,bundleHash,0);
        yield return request.SendWebRequest();

        var assetBundle = DownloadHandlerAssetBundle.GetContent(request);
        
        
        List<Hash128> listOfCachedVersions=new List<Hash128>();
        Caching.GetCachedVersions(assetBundle.name,listOfCachedVersions);

        //if(this was't i want bundle)
        {
            

            Caching.currentCacheForWriting = Caching.GetCacheAt(Caching.cacheCount);
            
            Caching.RemoveCache(newCache);
            
            
            for (var i = 0; i < listOfCachedVersions.Count; i++)
            {
                request = UnityWebRequestAssetBundle.GetAssetBundle(uri, listOfCachedVersions[i]);
                yield return request.SendWebRequest();
                assetBundle = DownloadHandlerAssetBundle.GetContent(request);
//                if (AssetBundleContainsAssetIWantToLoad(bundle))
//                    break;
                
            }


        }
//        //This is if we only want to keep 5 local caches at any time
//        if (Caching.cacheCount > 5)
//            Caching.RemoveCache(Caching.GetCacheAt(1));    
        


        yield return null;
    }
}