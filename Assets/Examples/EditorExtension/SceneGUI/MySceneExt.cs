using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;

#endif

public class MySceneExt : MonoBehaviour
{


    public float areaRadius = 1;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}



#if UNITY_EDITOR


[CustomEditor(typeof(MySceneExt))]
public class SceneExtEditor : Editor
{

    private MySceneExt _target
    {
        get { return target as MySceneExt; }
    }
    
    private void OnSceneGUI()
    {
        Handles.Label(_target.transform.position,_target.name+":"+_target.transform.position);
        Handles.BeginGUI();
        GUILayout.TextArea("11111");

        EditorGUILayout.Slider("aadsa", 1, 0, 1);
        
        GUILayout.BeginArea(new Rect(40,140,200,120));
        GUILayout.Label("hhhhh");
        
        GUILayout.EndArea();
        Handles.EndGUI();

        Handles.RadiusHandle(Quaternion.identity, _target.transform.position, 1);
        Handles.color = Handles.yAxisColor;
//        Handles.SphereHandleCap(0,_target.transform.position,Quaternion.identity,1,EventType.Repaint );
//        HandleUtility.AddDefaultControl(GUIUtility.GetControlID(FocusType.Passive));
    }
}

#endif