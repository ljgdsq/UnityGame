using System.Collections;
using System.Collections.Generic;
using UnityEditorInternal;
using UnityEngine;
#if UNITY_EDITOR
    using UnityEditor;
#endif
public class SimpleRecorderableDemo : MonoBehaviour
{
    
    public List<string> strList;

    
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

[CustomEditor(typeof(SimpleRecorderableDemo))]
public class RecorderableListDemoEditor1 : Editor
{
    private ReorderableList _reorderableList;

    private bool header=true;
    private bool dragable=true;
    private bool addBtn=true;
    private bool removeBtn=true;
    private void OnEnable()
    {
        _reorderableList=new ReorderableList(serializedObject,serializedObject.FindProperty("strList"),dragable,header,addBtn,removeBtn);

        _reorderableList.drawHeaderCallback = (rect) =>
        {
            GUI.Label(rect,"ReorderableList Header");
        };
        _reorderableList.onAddCallback = (list) =>
        {
            if (list.serializedProperty!=null)
            {
                list.serializedProperty.arraySize++;
                list.index = list.serializedProperty.arraySize - 1;
                SerializedProperty item = list.serializedProperty.GetArrayElementAtIndex(list.index);
                item.stringValue = "Defaule";
            }
            else
            {
                ReorderableList.defaultBehaviours.DoAddButton(list);
            }
            
        };
        _reorderableList.onRemoveCallback = (list) =>
        {

            if (EditorUtility.DisplayDialog("warrn","do you want remove ?","Remove","Cancel"))
            {
                ReorderableList.defaultBehaviours.DoRemoveButton(list);
            }
        };
        _reorderableList.drawElementCallback = (rect, index, selected, focus) =>
        {
            
            SerializedProperty item = _reorderableList.serializedProperty.GetArrayElementAtIndex(index);
            rect.height = EditorGUIUtility.singleLineHeight;
            rect.y += 2;
            EditorGUI.PropertyField(rect, item, new GUIContent("Element " + index));

        };


    }

    public override void OnInspectorGUI()
    {
        serializedObject.Update();

        _reorderableList.DoLayoutList();
        serializedObject.ApplyModifiedProperties();
    }
}
#endif
