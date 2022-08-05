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
       yield return SystemManager.Instance.Init();
       yield return WindowManager.Instance.Init();
    }


    public IEnumerator ReloadApplication()
    {
        AssetBundleManager.Instance.Init();
        SystemManager.Instance.DestroySingleTon();
        yield return SystemManager.Instance.Init();
        WindowManager.Instance.DestroySingleTon();
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