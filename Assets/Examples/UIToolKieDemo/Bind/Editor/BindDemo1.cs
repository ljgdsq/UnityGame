using UnityEditor;
using UnityEngine;
using UnityEngine.UIElements;
using UnityEditor.UIElements;


public class BindDemo1 : EditorWindow
{
    [MenuItem("Window/UI Toolkit/BindDemo1")]
    public static void ShowExample()
    {
        BindDemo1 wnd = GetWindow<BindDemo1>();
        wnd.titleContent = new GUIContent("BindDemo1");
    }

    private TextField _textField;

    public void CreateGUI()
    {
        // Each editor window contains a root VisualElement object
        VisualElement root = rootVisualElement;

        // Import UXML
        var visualTree =
            AssetDatabase.LoadAssetAtPath<VisualTreeAsset>("Assets/Examples/UIToolKieDemo/Bind/Editor/BindDemo1.uxml");
        VisualElement labelFromUXML = visualTree.Instantiate();
        root.Add(labelFromUXML);


        _textField = root.Q<TextField>("txt_input");

        if (_textField != null)
        {
            Debug.Log("find!");
//            _textField.bindingPath = "m_Name";
        }

        OnSelectionChange();
    }

    private void OnSelectionChange()
    {
        var selected = Selection.activeObject;
        if (selected)
        {
            var sob=new SerializedObject(selected);
            rootVisualElement.Bind(sob);
        }
        else
        {
            rootVisualElement.Unbind();
            _textField.value = "";
        }


    }
}