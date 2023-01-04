using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor.Android;
using UnityEngine;

public class AndroidGradleProcess : IPostGenerateGradleAndroidProject
{
 
    public int callbackOrder { get; }
    public void OnPostGenerateGradleAndroidProject(string path)
    {
//        var filePath = Path.Combine(Application.dataPath,"Plugins/Android/a.gradle");
//        var desPath = Path.Combine(path, "a.gradle");
//        Debug.LogError($"copy from {filePath} to {desPath}");
//        File.Copy(filePath,desPath);
        
    }
}
