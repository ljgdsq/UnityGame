using System.Collections;
using System.Collections.Generic;
using UnityEditor;

#if UNITY_EDITOR
    
using UnityEngine;

[CustomEditor(typeof(InspectorDemo))]
public class MyInspector : Editor
{
    
    
//    public override void OnInspectorGUI()
//    {
//        var obj = target as InspectorDemo;
//        obj.iValue = EditorGUILayout.IntField("iValue", obj.iValue);
//        obj.strValue = EditorGUILayout.TextField("strValue", obj.strValue);
//        obj.colorValue = EditorGUILayout.ColorField("colorValue", obj.colorValue);
//        obj.texValue =(Texture2D) EditorGUILayout.ObjectField("texValue", obj.texValue, typeof(Texture2D));
//
//        if (GUI.changed)
//        {
//              EditorUtility.SetDirty(target);
//        }
//        
//    }
//    


    private SerializedProperty intValue;
    private SerializedProperty strValue;
    private SerializedProperty colorValue;
    private SerializedProperty texValue;


    private void OnEnable()
    {
        intValue = serializedObject.FindProperty("iValue");
        strValue = serializedObject.FindProperty("strValue");
        colorValue = serializedObject.FindProperty("colorValue");
        texValue = serializedObject.FindProperty("texValue");
    }


    public override void OnInspectorGUI()
    {
        serializedObject.Update();

        EditorGUILayout.PropertyField(intValue);
        EditorGUILayout.PropertyField(strValue);
        EditorGUILayout.PropertyField(colorValue);
        EditorGUILayout.PropertyField(texValue);

        serializedObject.ApplyModifiedProperties();

//        if (GUILayout.Button("change str"))
        if (GUI.Button(GUILayoutUtility.GetRect(new GUIContent( "change str"),  GUI.skin.button, new GUILayoutOption[]{}),"change str"))
        {
             var obj = target as InspectorDemo;
            obj.strValue ="random"+ Random.Range(0, 99);
           EditorUtility.SetDirty(target);
        }

        
        
        
    }
    
    
    
    
}
#endif
