using UnityEditor;
using UnityEngine;
using UnityEngine.UIElements;
using UnityEditor.UIElements;


public class BindingPropertyDemo : EditorWindow
{
    [MenuItem("Window/UI Toolkit/BindingPropertyDemo")]
    public static void ShowExample()
    {
        BindingPropertyDemo wnd = GetWindow<BindingPropertyDemo>();
        wnd.titleContent = new GUIContent("BindingPropertyDemo");
    }
    private TextField _textField;

    public void CreateGUI()
    {
        // Each editor window contains a root VisualElement object
        VisualElement root = rootVisualElement;

        // Import UXML
        var visualTree = AssetDatabase.LoadAssetAtPath<VisualTreeAsset>("Assets/Examples/UIToolKieDemo/Bind/Editor/BindingPropertyDemo.uxml");
        VisualElement labelFromUXML = visualTree.Instantiate();
        root.Add(labelFromUXML);
        _textField = root.Q<TextField>("txt_input");
        
        OnSelectionChange();
        
    }

    private void OnSelectionChange()
    {
        var selected = Selection.activeObject;
        if (selected)
        {
            var sob=new SerializedObject(selected);
            var prop=sob.FindProperty("m_Name");
            _textField.BindProperty(prop);
        }
    }
}