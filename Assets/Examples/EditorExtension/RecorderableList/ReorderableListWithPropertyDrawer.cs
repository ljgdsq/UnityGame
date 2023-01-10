using System;
using System.Collections;
using System.Collections.Generic;
using UnityEditorInternal;
using UnityEngine;
#if UNITY_EDITOR
    using UnityEditor;
#endif


[Serializable]
public class PlayerItem
{
    [SerializeField]
    public Texture icon;
        
    [SerializeField]
    public GameObject prefab;
        
        
    [SerializeField]
    public string name;
        
    [SerializeField]
    public int attack;
}
public class ReorderableListWithPropertyDrawer : MonoBehaviour
{
    



    public List<PlayerItem> list;
    
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

[CustomEditor(typeof(ReorderableListWithPropertyDrawer))]
public class RecorderableListDemoEditor2 : Editor
{
    private ReorderableList _reorderableList;

    private bool header=true;
    private bool dragable=true;
    private bool addBtn=true;
    private bool removeBtn=true;
    private void OnEnable()
    {
        
        
        _reorderableList=new ReorderableList(serializedObject,serializedObject.FindProperty("list"),dragable,header,addBtn,removeBtn);
        _reorderableList.elementHeight = 68;
        
        _reorderableList.drawHeaderCallback = (rect) =>
        {
            GUI.Label(rect,"ReorderableList Header");
        };
        _reorderableList.onAddCallback = (list) =>
        {
//            if (list.serializedProperty!=null)
//            {
//                list.serializedProperty.arraySize++;
//                list.index = list.serializedProperty.arraySize - 1;
//                SerializedProperty item = list.serializedProperty.GetArrayElementAtIndex(list.index);
////                item.stringValue = "Defaule";
//            }
//            else
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
            rect.height -=4;
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


[CustomPropertyDrawer(typeof(PlayerItem))] 
public class PlayerItemDrawer : PropertyDrawer
{
    public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
    {

//        var pp=new EditorGUI.PropertyScope(position, label, property);
       using (new EditorGUI.PropertyScope(position, label, property))
        {
            EditorGUIUtility.labelWidth = 40;

            position.height = EditorGUIUtility.singleLineHeight;

            var iconRect = new Rect(position)
            {
                width = 64,
                height = 64
            };
            
            var prefabRect = new Rect(position)
                {
                    width = position.width - 80,
                    x = position.x + 80
                };

            var nameRect = new Rect(prefabRect) 
            {
                y = prefabRect.y + EditorGUIUtility.singleLineHeight + 5
            };

            var attackSliderRect = new Rect(nameRect)
                {
                    y = nameRect.y + EditorGUIUtility.singleLineHeight + 5
                };
  
            
            var iconProperty = property.FindPropertyRelative("icon");
            var prefabProperty = property.FindPropertyRelative("prefab"); 
            var nameProperty = property.FindPropertyRelative("name");
            var attackProperty = property.FindPropertyRelative("attack");

            iconProperty.objectReferenceValue = EditorGUI.ObjectField(iconRect, iconProperty.objectReferenceValue, typeof(Texture), false);
            nameProperty.stringValue = EditorGUI.TextField(nameRect, nameProperty.displayName,nameProperty.stringValue);
            prefabProperty.objectReferenceValue = EditorGUI.ObjectField(prefabRect, prefabProperty.objectReferenceValue,typeof(GameObject),false);
            attackProperty.intValue = EditorGUI.IntSlider(attackSliderRect, attackProperty.intValue,0,100);

            

        }
        
    }
}


#endif