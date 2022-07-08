using CliWrap;
using UnityEditor;
using UnityEngine;

namespace EditorTool
{
    public static class Git
    {

        private const string GIT_BASH = @"D:\Git\git-bash.exe";
        [MenuItem("Git/Pull")]
        public static void Pull()
        {
            var path = Application.dataPath+ "/../";
            Cli.Wrap("git").WithWorkingDirectory(path).WithArguments("pull").ExecuteAsync();
        }
        [MenuItem("Git/Push")]
        public static void Push()
        {
            var path = Application.dataPath + "/../";
            Cli.Wrap("git push").WithWorkingDirectory(path).ExecuteAsync();
        }
        
        [MenuItem("Git/GitBashHere")]
        public static void GitBashHere()
        {
            var path = Application.dataPath + "/../";
            Cli.Wrap(@"D:\Git\git-bash.exe").WithWorkingDirectory(path).ExecuteAsync();
        }
    }
}