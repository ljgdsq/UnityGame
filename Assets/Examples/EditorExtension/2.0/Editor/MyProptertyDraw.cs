using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEditor.UIElements;
using UnityEngine;
using UnityEngine.UIElements;

[CustomPropertyDrawer(typeof(ProptertyDrawTest.Ingredient))]
public class MyProptertyDraw : PropertyDrawer
{
    public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
    {
        // Using BeginProperty / EndProperty on the parent property means that
        // prefab override logic works on the entire property.
        EditorGUI.BeginProperty(position, label, property);

        // Draw label
        position = EditorGUI.PrefixLabel(position, GUIUtility.GetControlID(FocusType.Passive), label);

        // Don't make child fields be indented
        var indent = EditorGUI.indentLevel;
        EditorGUI.indentLevel = 0;

        // Calculate rects
        var amountRect = new Rect(position.x, position.y, 30, position.height);
        var unitRect = new Rect(position.x + 35, position.y, 50, position.height);
        var nameRect = new Rect(position.x + 90, position.y, position.width - 90, position.height);

        // Draw fields - pass GUIContent.none to each so they are drawn without labels
        EditorGUI.PropertyField(amountRect, property.FindPropertyRelative("amount"), GUIContent.none);
        EditorGUI.PropertyField(unitRect, property.FindPropertyRelative("unit"), GUIContent.none);
        EditorGUI.PropertyField(nameRect, property.FindPropertyRelative("name"), GUIContent.none);

        // Set indent back to what it was
        EditorGUI.indentLevel = indent;

        EditorGUI.EndProperty();
    }

//    public override VisualElement CreatePropertyGUI(SerializedProperty property)
//    {
//        // Create property container element.
//        var container = new VisualElement();
//
//        // Create property fields.
//        var amountField = new PropertyField(property.FindPropertyRelative("amount"));
//        var unitField = new PropertyField(property.FindPropertyRelative("unit"));
//        var nameField = new PropertyField(property.FindPropertyRelative("name"), "Fancy Name");
//
//        // Add fields to the container.
//        container.Add(amountField);
//        container.Add(unitField);
//        container.Add(nameField);
//
//        return container;
//    }
}
