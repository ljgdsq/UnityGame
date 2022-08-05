using System.Collections;
using Base;
using Manager;
using Save;
using SAssetbundle;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

public class GameApplication:MonoSingleton<GameApplication>
{
    [RuntimeInitializeOnLoadMethod]
    private static void OnFirstLoad()
    {
        Instance.Init();

#if UNITY_EDITOR
        var scenes = EditorBuildSettings.scenes;
        foreach (var scene in scenes)
        {
            scene.enabled = true;
        }

        EditorBuildSettings.scenes = scenes;

#endif
    }

    private IEnumerator Start()
    {
#if JSON_PREFS_SAVE
        SaveGame.Instance.SetArchive(new PlayerPrefsJsonArchive());
#else 
        SaveGame.Instance.SetArchive(new BinaryArchive()); 
#endif
   
        SaveGame.Instance.Init();
        
        AssetBundleManager.Instance.Init();

        yield return AssetManager.AssetManager.LoadSelf();
       yield return WindowManager.Instance.Init();
    }



    
    /// <summary>
    /// get a Writeable path
    /// </summary>
    /// <returns></returns>
    public static string GetWriteablePath()
    {
        return Application.persistentDataPath;
    }
    
}