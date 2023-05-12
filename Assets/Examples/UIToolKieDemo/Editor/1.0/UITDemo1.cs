using UnityEditor;
using UnityEngine;
using UnityEngine.UIElements;
using UnityEditor.UIElements;


public class UITDemo1 : EditorWindow
{
    [MenuItem("Window/UI Toolkit/UITDemo1")]
    public static void ShowExample()
    {
        UITDemo1 wnd = GetWindow<UITDemo1>();
        wnd.titleContent = new GUIContent("UITDemo1");
    }

    public void CreateGUI()
    {
        // Each editor window contains a root VisualElement object
        VisualElement root = rootVisualElement;

        // VisualElements objects can contain other VisualElement following a tree hierarchy.
        VisualElement label = new Label("Hello World! From C#");
        root.Add(label);
        // Import UXML
        var visualTree = AssetDatabase.LoadAssetAtPath<VisualTreeAsset>("Assets/Examples/UIToolKieDemo/1.0/UITDemo1.uxml");
        VisualElement labelFromUXML = visualTree.Instantiate();
        root.Add(labelFromUXML);

//        var style=AssetDatabase.LoadAssetAtPath<StyleSheet>("Assets/Examples/UIToolKieDemo/1.0/UITDemo1.uss");
//        rootVisualElement.styleSheets.Add(style);
    }
}