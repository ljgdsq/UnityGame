using UnityEditor;
using UnityEngine;
using UnityEngine.UIElements;
using UnityEditor.UIElements;


public class PositioningTestWindow : EditorWindow
{
    [MenuItem("Window/UI Toolkit/PositioningTestWindow")]
    public static void ShowExample()
    {
        PositioningTestWindow wnd = GetWindow<PositioningTestWindow>();
        wnd.titleContent = new GUIContent("PositioningTestWindow");
    }

    public void CreateGUI()
    {
//        // Each editor window contains a root VisualElement object
//        VisualElement root = rootVisualElement;
//
//        // VisualElements objects can contain other VisualElement following a tree hierarchy.
//        VisualElement label = new Label("Hello World! From C#");
//        root.Add(label);
//
//        // Import UXML
//        var visualTree = AssetDatabase.LoadAssetAtPath<VisualTreeAsset>("Assets/Examples/UIToolKieDemo/PositioningTestWindow.uxml");
//        VisualElement labelFromUXML = visualTree.Instantiate();
//        root.Add(labelFromUXML);
//
//        // A stylesheet can be added to a VisualElement.
//        // The style will be applied to the VisualElement and all of its children.
//        var styleSheet = AssetDatabase.LoadAssetAtPath<StyleSheet>("Assets/Examples/UIToolKieDemo/PositioningTestWindow.uss");
//        VisualElement labelWithStyle = new Label("Hello World! With Style");
//        labelWithStyle.styleSheets.Add(styleSheet);
//        root.Add(labelWithStyle);

        for (int i =   0; i <2; i++)
        {
            var temp=new VisualElement();
            var tempStyle = temp.style;
            tempStyle.width = 70;
            tempStyle.height = 70;
            tempStyle.marginBottom  = 4;
            tempStyle.backgroundColor  = Color.gray;
            rootVisualElement.Add(temp);
        }
   
        var relative = new Label("Relative\nPos\n25, 0");
        relative.style.width = 70;
        relative.style.height = 70;
        relative.style.left = 25;
        relative.style.marginBottom = 2;
        relative.style.marginLeft = 2;
        relative.style.backgroundColor = new Color(0.2165094f, 0, 0.254717f);
        rootVisualElement.Add(relative);
        
        for (int i = 0; i < 2; i++)
        {
            var temp = new VisualElement();
            temp.style.width = 70;
            temp.style.height = 70;
            temp.style.marginBottom = 2;
            temp.style.backgroundColor = Color.gray;
            rootVisualElement.Add(temp);
        }
            
        var absolutePositionElement = new Label("Absolute\nPos\n25, 25");
        absolutePositionElement.style.position = Position.Absolute;
        absolutePositionElement.style.top = 25;
        absolutePositionElement.style.left = 25;
        absolutePositionElement.style.width = 70;
        absolutePositionElement.style.height = 70;
        absolutePositionElement.style.backgroundColor = Color.black;
        rootVisualElement.Add(absolutePositionElement);
    }
}