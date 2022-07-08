
using UnityEditor;
using UnityEngine;
using CliWrap;

namespace EditorTool
{
    public static class Explorer
    {
        [MenuItem("Tools/OpenExplorer")]
        public static void OpenExplorer()
        {

            var path = Application.dataPath + "/../";
            Cli.Wrap("explorer.exe").WithWorkingDirectory(path).WithArguments(".").ExecuteAsync();
        }
    }
}