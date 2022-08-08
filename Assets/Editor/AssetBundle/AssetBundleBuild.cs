using System.IO;
using System.Linq;
using Newtonsoft.Json;
using SAssetbundle;
using UnityEditor;
using UnityEngine;

public partial class AssetBundleBuildScript
{
    //BuildAssetBundleOptions.DisableWriteTypeTree  兼容不同版本ab包数据结构产生变化
    //BuildAssetBundleOptions.ChunkBasedCompression  lz4 压缩


//    public static IEnumerator DoSome()
//    {
//        BundleManifest bundleManifest = new BundleManifest();
//        AssetBundle.UnloadAllAssetBundles(true);
//
//        AssetDatabase.RemoveUnusedAssetBundleNames();
//        var path = Path.Combine("Assets/StreamingAssets", EditorUserBuildSettings.activeBuildTarget.ToString());
//        var assetBundle = AssetBundle.LoadFromFile(path);
//
//        var manifest = assetBundle.LoadAsset<AssetBundleManifest>("AssetBundleManifest");
//        var allAssetBundles = manifest.GetAllAssetBundles();
//        
//        foreach (var abName in allAssetBundles)
//        {
//            bundleManifest.AddBundleInfo(abName);
//            var abPath = Path.Combine(Application.streamingAssetsPath, abName);
//            var allLoadedAssetBundles = AssetBundle.GetAllLoadedAssetBundles();
//
//            if (abName.Contains("scene"))
//            {
////                yield return EditorSceneManager.UnloadSceneAsync("SceneA");
//                
////                yield return EditorSceneManager.CloseScene("SceneA");
////                yield return SceneManager.UnloadSceneAsync("SceneA");
//            }
//            
//            var bundle = AssetBundle.LoadFromFile(abPath);
//            if (abName.Contains("scene"))
//            {
//
//                for (int index = 0; index < SceneManager.sceneCount; ++index)
//                {
//                    var sceneArray = SceneManager.GetSceneAt(index); 
//                    Debug.Log(sceneArray);
//                }
//
//                EditorSceneManager.OpenScene("SceneA");
//                yield return null;
////                yield return EditorSceneManager.UnloadSceneAsync("SceneA");
//            }
//           
//            var allAssetNames = bundle.GetAllAssetNames();
//            var allScenePaths = bundle.GetAllScenePaths();
//
//            bundleManifest.AddBundleAsset(abName, allAssetNames.ToList());
//            bundle.Unload(true);
////            Object.DestroyImmediate(bundle);
//        }
//        bundleManifest.GenerateDependency();
//        
//        assetBundle.Unload(true);
//        var json = JsonConvert.SerializeObject(bundleManifest,Formatting.Indented);
//
//        File.WriteAllText(Application.dataPath + "main.json", json);
//
//        Debug.Log(json);
//    }


//    [MenuItem("Build/AnalysisDeps")]
//    public static void GetDeps()
//    {
//        var path = "Assets/StreamingAssets/" + EditorUserBuildSettings.activeBuildTarget + ".manifest";
//        var manifest = AssetDatabase.LoadAssetAtPath(path, typeof(TextAsset));
//        Debug.Log(manifest);
//    }
//
//
//    [MenuItem("Build/Special")]
//    public static void BuildS()
//    {
//        ClearPath();
//
//        AssetBundleBuild[] assetBundleBuild = new AssetBundleBuild[2];
//        assetBundleBuild[0] = new AssetBundleBuild();
//        assetBundleBuild[1] = new AssetBundleBuild();
//        assetBundleBuild[0].assetBundleName = "Cube";
//        assetBundleBuild[0].assetNames = new[]
//        {
//            "Assets/Test/Prefabs/Cube.prefab",
//        };
//
//        assetBundleBuild[1].assetBundleName = "mt";
//        assetBundleBuild[1].assetNames = new[]
//        {
//            "Assets/Test/New Folder/New Material.mat"
//        };
//
//
//        BuildPipeline.BuildAssetBundles(Application.streamingAssetsPath, assetBundleBuild, BuildAssetBundleOptions.None,
//            EditorUserBuildSettings.activeBuildTarget
//        );
//
//
//        AssetBundleManifest bundleManifest;
//
//        AssetBundle assetBundle;
//    }
//
//
//    [MenuItem("Build/Normal")]
//    public static void Build()
//    {
//        ClearPath();
//        BuildPipeline.BuildAssetBundles(Application.streamingAssetsPath, BuildAssetBundleOptions.None,
//            EditorUserBuildSettings.activeBuildTarget
//        );
//    }
//
//    [MenuItem("Build/Deterministic")]
//    public static void BuildD()
//    {
//        ClearPath();
//        BuildPipeline.BuildAssetBundles(Application.streamingAssetsPath,
//            BuildAssetBundleOptions.DeterministicAssetBundle,
//            EditorUserBuildSettings.activeBuildTarget
//        );
//    }


    //    [MenuItem("Build/BuildInfo")]
//    public static void BuildInfo()
//    {
//        EditorCoroutineUtility.StartCoroutineOwnerless(DoSome());
//    }

    [MenuItem("Build/BuildManifest")]
    public static void BuildInfo()
    {
        AssetDatabase.RemoveUnusedAssetBundleNames();
        AssetBundle.UnloadAllAssetBundles(true);
        var path = Path.Combine("Assets/StreamingAssets", EditorUserBuildSettings.activeBuildTarget.ToString());
        var assetBundle = AssetBundle.LoadFromFile(path);
        var manifest = assetBundle.LoadAsset<AssetBundleManifest>("AssetBundleManifest");
        var allAssetBundles = manifest.GetAllAssetBundles();

        BundleManifest bundleManifest = new BundleManifest();
        foreach (var abName in allAssetBundles)
        {

            var abPath = Path.Combine(Application.streamingAssetsPath, abName);
            BuildPipeline.GetCRCForAssetBundle(abPath, out uint crc);
            BuildPipeline.GetHashForAssetBundle(abPath, out Hash128 hash);
            FileInfo fileInfo=new FileInfo(abPath);
            bundleManifest.AddBundleInfo(abName,crc,hash.ToString(),fileInfo.Length);

            var bundle = AssetBundle.LoadFromFile(abPath);
        
            
            var allAssetNames = bundle.GetAllAssetNames();
            bundleManifest.AddBundleAsset(abName, allAssetNames.ToList());
            
            if (bundle.isStreamedSceneAssetBundle)
            {
                var allScenePaths = bundle.GetAllScenePaths();
                bundleManifest.AddScenes(allScenePaths.ToList());
                bundleManifest.AddBundleAsset(abName, allScenePaths.ToList());
            }
      
      
            bundle.Unload(true);
            Object.DestroyImmediate(bundle);
        }

        bundleManifest.GenerateDependency();

        assetBundle.Unload(true);
        var json = JsonConvert.SerializeObject(bundleManifest, Formatting.Indented);

        File.WriteAllText(Path.Combine(Application.dataPath, "../","AssetBundles","main.json"), json);

        Debug.Log(json);
        var assetPath = Path.Combine(Application.dataPath, "Manifest", BundleManifest.ManifestName);
        File.WriteAllText(assetPath,json);
        AssetDatabase.ImportAsset("Assets/Manifest/main.txt");
        AssetDatabase.SaveAssets();
        
        AssetBundleBuild assetBundleBuild=new AssetBundleBuild();
        assetBundleBuild.assetBundleName = "Manifest.ab";
        assetBundleBuild.assetNames = new[] {"Assets/Manifest/main.txt" };
        BuildPipeline.BuildAssetBundles(Application.streamingAssetsPath,
            new AssetBundleBuild[] {assetBundleBuild},
            BuildAssetBundleOptions.ChunkBasedCompression | BuildAssetBundleOptions.DisableWriteTypeTree,
            EditorUserBuildSettings.activeBuildTarget);

    }

    [MenuItem("Build/LoadManifest")]

    public static void TestLoadManifest()
    {
        AssetBundleManager.Instance.LoadManifest();
    }

    [MenuItem("Build/ExportPackage")]

    public static void ExportPackage()
    {
       EditorUserBuildSettings.exportAsGoogleAndroidProject = true;
       BuildPipeline.BuildPlayer(EditorBuildSettings.scenes, Application.dataPath +"/../Build/", BuildTarget.Android, BuildOptions.None);
        
    }
    [MenuItem("Build/UploadDLC")]
    public static void UploadDLC()
    {
        //todo 
        var dlcPath = Application.streamingAssetsPath;
        var hostPath = "E://FileServer/V1.0/";
        if (Directory.Exists(hostPath))
        {
            Directory.Delete(hostPath,true);
        }
        Directory.Move(dlcPath,hostPath);
        
    }
    
    private static void ClearPath()
    {
        if (Directory.Exists(Application.streamingAssetsPath))
        {
            Directory.Delete(Application.streamingAssetsPath, true);
        }

        Directory.CreateDirectory(Application.streamingAssetsPath);
    }
}