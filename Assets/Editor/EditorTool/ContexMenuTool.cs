using UnityEditor;
using UnityEngine;

namespace EditorTool
{
    public class ContexMenuTool
    {
        
        [MenuItem("Assets/CopyObjectPath",false,7)]
        public static void CopyPath()
        {

            var projectPath = Application.dataPath;
            
            var objects = Selection.objects;
            if (objects!=null && objects.Length>=1)
            {
                foreach (var o in objects)
                {
                    var assetPath = AssetDatabase.GetAssetPath(o);
                    var relativePath=assetPath.Substring("Assets".Length);
                    Debug.Log(assetPath);
                }
            }
        }
    }
}