using System.Collections;
using System.Collections.Generic;
using UnityEngine;
#if UNITY_EDITOR
using UnityEditor;

#endif
public class EditorTest : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
    }


#if UNITY_EDITOR

    [MenuItem("Test/DeleteSelected",true)]

    private static bool ValidateDelete()
    {
        if (Selection.objects.Length<=2)
        {
            Debug.LogWarning("can't del");
            return false;
        }

        return true;

    }
    

    [MenuItem("Test/DeleteSelected %_k",false)]
    private static void DeleteSelected()
    {
        foreach (var o in Selection.objects)
        {
            Debug.LogWarning("DeleteSelected:"+o.name);
            Undo.DestroyObjectImmediate(o);
        }
    }

    [MenuItem("CONTEXT/MonoBehaviour/ContextMenu1")]
    public static void ContextMenu()
    {
        
    }
    
    [ContextMenu("ContextFunctionMenu")]
    public  void ContextFunctionMenu()
    {
        
    }
    
    
    
    
    
#endif


#if UNITY_EDITOR

    private void SetI()
    {
        i = 99;
    }
    [ContextMenuItem("handle i", nameof(SetI))]

#endif
    
    
    
    
    
    public int i;
    
    
    private void SetJ()
    {
        j = 99;
    }
    [ContextMenuItem("handle j", nameof(SetJ))]

    public int j;

}