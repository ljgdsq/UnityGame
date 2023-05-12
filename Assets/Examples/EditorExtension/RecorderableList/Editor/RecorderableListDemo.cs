using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR
    using UnityEditor;
#endif

public class RecorderableListDemo : MonoBehaviour
{


    public List<string> strList;

}
#if UNITY_EDITOR
[CustomEditor(typeof(RecorderableListDemo))]
public class RecorderableListDemoEditor : Editor
{

    private SerializedProperty strList;
    private void OnEnable()
    {
        strList = serializedObject.FindProperty("strList");
    }

    public override void OnInspectorGUI()
    {
        
        serializedObject.Update();


        EditorGUILayout.PropertyField(strList, true);
        serializedObject.ApplyModifiedProperties();
    }
}

#endif