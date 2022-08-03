using System.IO;
using UnityEditor;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using UnityEngine;
using UnityEngine.SceneManagement;

public class BuildPackageProcessor : IPreprocessBuildWithReport
{
    public int callbackOrder { get; }

    public void OnPreprocessBuild(BuildReport report)
    {
        
        //TODO
//        foreach (var buildSettingsScene in EditorBuildSettings.scenes)
//        {
//            if (!buildSettingsScene.path.Contains("splash"))
//            {
//                buildSettingsScene.enabled = false;
//            }
//        }
        
        var reportFiles = report.files;
        var reportSteps = report.steps;
        Debug.LogError("BuildPackageProcessor");

        var streamingAssetsPath = Application.streamingAssetsPath;

        var files = Directory.GetFiles(streamingAssetsPath, "*.manifest");
        foreach (var file in files)
        {
            File.Delete(file);
        }


        files = Directory.GetFiles(streamingAssetsPath, "*.meta");
        foreach (var file in files)
        {
            File.Delete(file);
        }
    }
}