using System.Collections;
using System.Collections.Generic;
using UnityEngine;
#if UNITY_EDITOR
    
using UnityEditor;

public class PopWindowExample : EditorWindow
{

    private Rect popRect=new Rect(10,100,0,0);
    private static PopWindow1 _popWindow1=new PopWindow1();
    [MenuItem("MyWindow/PopWindowExample")]
    private static void ShowWindow()
    {
        GetWindow<PopWindowExample>().Show();
        _popWindow1.editorWindow.titleContent=new GUIContent("pop win 111");
    }


    private void OnGUI()
    {
        if (GUILayout.Button("Show",EditorStyles.boldLabel))
        {
            PopupWindow.Show(popRect,_popWindow1);
        }

        if (Event.current.type == EventType.Repaint)
        {
            popRect = GUILayoutUtility.GetLastRect();
        }
        
    }
}


public class PopWindow1 : PopupWindowContent
{
    public override void OnGUI(Rect rect)
    {
        EditorGUILayout.LabelField("pop win");
    }


    public override Vector2 GetWindowSize()
    {
        return new Vector2(200,200);
    }
}

#endif
