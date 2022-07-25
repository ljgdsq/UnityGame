using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

public class AssetBundleBuild 
{
    
    //BuildAssetBundleOptions.DisableWriteTypeTree  兼容不同版本ab包数据结构产生变化
    //BuildAssetBundleOptions.ChunkBasedCompression  lz4 压缩

    [MenuItem("Build/Normal")]
    public static void Build()
    {

        ClearPath();
        BuildPipeline.BuildAssetBundles(Application.streamingAssetsPath, BuildAssetBundleOptions.None,
            EditorUserBuildSettings.activeBuildTarget
        );
    }
    
    [MenuItem("Build/Deterministic")]
    public static void BuildD()
    {
        ClearPath();
        BuildPipeline.BuildAssetBundles(Application.streamingAssetsPath, BuildAssetBundleOptions.DeterministicAssetBundle,
            EditorUserBuildSettings.activeBuildTarget
        );
    }

    private static void ClearPath()
    {
        
        
        return;
        if (Directory.Exists(Application.streamingAssetsPath))
        {
            Directory.Delete(Application.streamingAssetsPath,true);
        }
     
        Directory.CreateDirectory(Application.streamingAssetsPath);
    
    }
}
