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
        var scenes = EditorBuildSettings.scenes;
        foreach (var buildSettingsScene in scenes)
        {
            if (!buildSettingsScene.path.Contains("splash"))
            {
                buildSettingsScene.enabled = false;
            }
        }

        EditorBuildSettings.scenes = scenes;


//        Debug.LogError("BuildPackageProcessor");

        var streamingAssetsPath = Application.streamingAssetsPath;
        if (Directory.Exists(streamingAssetsPath))
        {
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
}