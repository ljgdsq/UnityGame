//using System.Collections.Generic;
//using UnityEditor;
//using UnityEditor.Build.Pipeline;
//using UnityEngine;
//
//namespace AssetBundleBrowser.AssetBundleDataSource
//{
//    public class AssetDatabaseNewABDataSource: ABDataSource
//    {
//        
//        public static List<ABDataSource> CreateDataSources()
//        {
//            var op = new AssetDatabaseNewABDataSource();
//            var retList = new List<ABDataSource>();
//            retList.Add(op);
//            return retList;
//        }
//        
//        public string Name {
//            get {
//                return "New Build";
//            }
//        }
//
//        public string ProviderName {
//            get {
//                return "Scriptable Build Pipeline";
//            }
//        }
//        public string[] GetAssetPathsFromAssetBundle(string assetBundleName)
//        {
//            return AssetDatabase.GetAssetPathsFromAssetBundle(assetBundleName);
//        }
//
//        public string GetAssetBundleName(string assetPath)
//        {
//            var importer = AssetImporter.GetAtPath(assetPath);
//            if (importer == null) {
//                return string.Empty;
//            }
//            var bundleName = importer.assetBundleName;
//            if (importer.assetBundleVariant.Length > 0) {
//                bundleName = bundleName + "." + importer.assetBundleVariant;
//            }
//            return bundleName;
//        }
//
//        public string GetImplicitAssetBundleName(string assetPath)
//        {
//            return AssetDatabase.GetImplicitAssetBundleName (assetPath);
//        }
//
//        public string[] GetAllAssetBundleNames()
//        {
//            return AssetDatabase.GetAllAssetBundleNames ();
//        }
//
//        public bool IsReadOnly()
//        {
//            return false;
//        }
//
//      
//        public void SetAssetBundleNameAndVariant (string assetPath, string bundleName, string variantName) {
//            AssetImporter.GetAtPath(assetPath).SetAssetBundleNameAndVariant(bundleName, variantName);
//        }
//
//        public void RemoveUnusedAssetBundleNames() {
//            AssetDatabase.RemoveUnusedAssetBundleNames ();
//        }
//
//        public bool CanSpecifyBuildTarget { 
//            get { return true; } 
//        }
//        public bool CanSpecifyBuildOutputDirectory { 
//            get { return true; } 
//        }
//
//        public bool CanSpecifyBuildOptions { 
//            get { return true; } 
//        }
//
//        public bool BuildAssetBundles(ABBuildInfo info)
//        {
//            if(info == null)
//            {
//                Debug.Log("Error in build");
//                return false;
//            }
//
//            
//            var manifest = CompatibilityBuildPipeline.BuildAssetBundles(info.outputDirectory, info.options,  info.buildTarget);
//
//            if (manifest==null)
//            {
//                Debug.LogError("build failed!");
//                return false;
//            }
//            foreach(var assetBundleName in manifest.GetAllAssetBundles())
//            {
//                if (info.onBuild != null)
//                {
//                    info.onBuild(assetBundleName);
//                }
//            }
//            
//            
//            return true;
//        }
//    }
//}