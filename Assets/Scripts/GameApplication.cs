using Base;
using Save;

using UnityEngine;
public class GameApplication:MonoSingleton<GameApplication>
{
    [RuntimeInitializeOnLoadMethod]
    private static void OnFirstLoad()
    {
        Instance.Init();
    }

    public override void Init()
    {

#if JSON_PREFS_SAVE
        SaveGame.Instance.SetArchive(new PlayerPrefsJsonArchive());
#else 
        SaveGame.Instance.SetArchive(new BinaryArchive()); 
#endif
   
        
        
        SaveGame.Instance.Init();
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