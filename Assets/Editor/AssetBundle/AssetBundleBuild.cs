using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

public partial class AssetBundleBuildScript
{
    
    //BuildAssetBundleOptions.DisableWriteTypeTree  兼容不同版本ab包数据结构产生变化
    //BuildAssetBundleOptions.ChunkBasedCompression  lz4 压缩

    [MenuItem("Build/GetDeps")]
    public static void GetDeps()
    {
        var assetPath = @"Assets/Test/Prefabs/Image.prefab";
        var dependencies = AssetDatabase.GetDependencies(assetPath);
        foreach (var dependency in dependencies)
        {
            Debug.Log(dependency);
        }
    }
    
    
    [MenuItem("Build/Special")]
    public static void BuildS()
    {

        ClearPath();
    
        AssetBundleBuild[] assetBundleBuild=new AssetBundleBuild[2];
        assetBundleBuild[0]=new AssetBundleBuild();
        assetBundleBuild[1]=new AssetBundleBuild();
        assetBundleBuild[0].assetBundleName = "Cube";
        assetBundleBuild[0].assetNames = new []
        {
            "Assets/Test/Prefabs/Cube.prefab",
    
        };
        
        assetBundleBuild[1].assetBundleName = "mt";
        assetBundleBuild[1].assetNames = new []
        {
       
            "Assets/Test/New Folder/New Material.mat"
        };
        
        
        BuildPipeline.BuildAssetBundles(Application.streamingAssetsPath,assetBundleBuild,BuildAssetBundleOptions.None,
            EditorUserBuildSettings.activeBuildTarget
        );


        AssetBundleManifest bundleManifest;

        AssetBundle assetBundle;
        
   
    }

    
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
        
        
     
        if (Directory.Exists(Application.streamingAssetsPath))
        {
            Directory.Delete(Application.streamingAssetsPath,true);
        }
     
        Directory.CreateDirectory(Application.streamingAssetsPath);
    
    }
}
