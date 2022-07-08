using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using CliWrap;
using UnityEngine;
using UnityEditor;
namespace EditorTool
{
 
    public class ImageCompressWindow : EditorWindow
    {
        
        [MenuItem("Tools/ImageCompressionWindow")]
        public static void AutoCompressAll()
        {
            CreateInstance<ImageCompressWindow>().Show();
        }
        
        private float _min = 40;
        private float _max = 90;
        
        private bool _isToolExist;
        private string _exePath;
        private void OnEnable()
        {
            _exePath =Application.dataPath+ "/Editor/Thirdpart/ImageCompress/pngquant.exe";
            _isToolExist = File.Exists(_exePath);

            _min=EditorPrefs.GetFloat("ImageCompressWindow.min", _min);
            _max=EditorPrefs.GetFloat("ImageCompressWindow.max", _max);

        }

        private void OnGUI()
        {
            if (!_isToolExist)
            {
                
                EditorGUILayout.HelpBox("no tool found!",MessageType.Error);
                return;
            }

            _min = EditorGUILayout.Slider("min", _min, 1, 50);
            _max = EditorGUILayout.Slider("max", _max, 50, 99);

            if (GUILayout.Button("CompressAll"))
            {

                var allFile = FindAllImage();
                
                
                List<string> errorFiles=new List<string>();

                
                EditorUtility.DisplayProgressBar("CompressAllImage","",0);

                
                
                
//                async Task Comprees(string inFile,string outFile )
//                {
//                    var args = $"--f --ordered --quality={_min}-{_max} {inFile} -o {outFile}";
//                    Command command = Cli.Wrap(_exePath).WithArguments(args);
//                    
//                    Debug.Log(command.ToString());
//                    try
//                    {
//                    
//                        CommandResult executeAsync = await command.ExecuteAsync();
//                        if (executeAsync.ExitCode != 0)
//                        {
//                            errorFiles.Add(inFile);
//                        }
//                    }
//                    catch (Exception e)
//                    {
//                        Console.WriteLine(e);
//                        throw;
//                    }
//
//                };

                   
                int count = 0;
                int sum = allFile.Count;
                foreach (var file in allFile)
                {
                    var outFile = file;
//                    var task=Task.Run(()=>Comprees(file,outFile));
//                    Task.WaitAll(task);
                    var cmd = $"{_exePath} --f --ordered --quality={_min}-{_max} {file} -o {outFile}";

                    var ret=CMDer.Run(".", cmd, 0);
                    if (!ret)
                    {
                        Debug.LogError(cmd);
                        errorFiles.Add(file);
                    }
                    else
                    {
                        Debug.Log(cmd);
                    }
                    count++;
                    EditorUtility.DisplayProgressBar("CompressAllImage",$"compressing......{count}/{sum} ",count*1.0f/sum);
                    
                }
                
                EditorUtility.ClearProgressBar();
            }
        }

        private static List<string> FindAllImage()
        {
            var images = Directory.GetFiles(Application.dataPath,"*.png",SearchOption.AllDirectories);
            List<string> list=new List<string>();
            foreach (var image in images)
            {
                list.Add(image.Replace("\\","/"));
            }
            return list;
        }

        private void OnDestroy()
        {
            EditorPrefs.SetFloat("ImageCompressWindow.min",_min);
            EditorPrefs.SetFloat("ImageCompressWindow.max",_max);
        }
    }
}