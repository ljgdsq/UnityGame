using System.Linq;
using UnityEngine;

namespace SAssetbundle
{
    public class AssetBundleLoadCache
    {
        public bool IsBundleLoaded(string bundleName)
        {
            return AssetBundle.GetAllLoadedAssetBundles().Any((bundle) => { return bundle.name.Equals(bundleName); });
        }

        public AssetBundle GetCachedBundle(string bundleName)
        {
           return AssetBundle.GetAllLoadedAssetBundles().FirstOrDefault((bundle) => { return bundle.name.Equals(bundleName); });
        }
    }
}