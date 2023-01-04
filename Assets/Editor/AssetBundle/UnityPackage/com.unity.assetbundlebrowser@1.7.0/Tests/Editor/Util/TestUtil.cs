using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using UnityEditor;
using UnityEngine;

namespace Assets.Editor.Tests.Util
{
    class TestUtil
    {
  
        public static string CreatePrefabWithBundleAndVariantName(string bundleName, string variantName, string name = "Cube")
        {
            string path = "Assets/" + UnityEngine.Random.Range(0, 10000) + ".prefab";
            GameObject instance = GameObject.CreatePrimitive(PrimitiveType.Cube);

            GameObject go = PrefabUtility.CreatePrefab(path, instance);
            instance.name = name;
            AssetImporter.GetAtPath(path).SetAssetBundleNameAndVariant(bundleName, variantName);
            PrefabUtility.MergeAllPrefabInstances(go);
            return path;
        }

        static void DestroyPrefabsAndRemoveUnusedBundleNames(IEnumerable<string> prefabPaths)
        {
            foreach (string prefab in prefabPaths)
            {
                AssetDatabase.DeleteAsset(prefab);
            }

            AssetDatabase.RemoveUnusedAssetBundleNames();
        }
    }
}
