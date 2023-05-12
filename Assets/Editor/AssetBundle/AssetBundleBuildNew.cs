using System.IO;
using UnityEditor;
using UnityEditor.Build.Pipeline;
using UnityEngine;

namespace DefaultNamespace
{
    public class AssetBundleBuildNew
    {
        [MenuItem("Build/BuildAssetBundleNew")]
        public static void BuildAssetBundle()
        {

            var outputPath =Path.Combine(Application.dataPath + "/../"+"SBPBuild");
            var options = BuildAssetBundleOptions.None;

                options |= BuildAssetBundleOptions.ChunkBasedCompression;

          //    options |= BuildAssetBundleOptions.ForceRebuildAssetBundle;

            Directory.CreateDirectory(outputPath);

            // Replaced BuildPipeline.BuildAssetBundles with CompatibilityBuildPipeline.BuildAssetBundles here
            var manifest = CompatibilityBuildPipeline.BuildAssetBundles(outputPath, options,  EditorUserBuildSettings.activeBuildTarget);
            if (manifest==null)
            {
                Debug.LogError("build failed!");
                return;
            }
            var allAssetBundlesWithVariant = manifest.GetAllAssetBundlesWithVariant();

        }
    }
}