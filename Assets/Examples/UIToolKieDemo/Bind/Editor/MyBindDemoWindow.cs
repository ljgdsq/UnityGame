//
//
//
//
//using UnityEditor;
//using UnityEngine;
//using UnityEngine.UIElements;
//
//public class MyBindDemoWindow : EditorWindow
//{
//    private TextField _textField;
//
//    [MenuItem("Window/UI Toolkit/MyBindDemoWindow")]
//    public static void ShowExample()
//    {
//        MyBindDemoWindow wnd = GetWindow<MyBindDemoWindow>();
//        wnd.titleContent = new GUIContent("MyBindDemoWindow");
//    }
//
//    public void CreateGUI()
//    {
//        // Each editor window contains a root VisualElement object
//        VisualElement root = rootVisualElement;
//
//        // Import UXML
//        var visualTree = AssetDatabase.LoadAssetAtPath<VisualTreeAsset>("Assets/Examples/UIToolKieDemo/Bind/Editor/MyBindDemoWindow.uxml");
//        VisualElement labelFromUXML = visualTree.Instantiate();
//        root.Add(labelFromUXML);
////        _textField = root.Q<TextField>("TextField");
////        if (_textField!=null)
////        {
////            _textField.bindingPath = "m_Name";
//// 
////        }
////        else
////        {
////            Debug.LogError("null _textField");
////        }
////  
////
////        OnSelectionChange();
//
//    }
//
//    private void OnSelectionChange()
//    {
////        var activeObject = Selection.activeObject;
////        if (activeObject)
////        {
////            var sob = new SerializedObject(activeObject);
////            rootVisualElement.Bind(sob);
////        }
////        else
////        {
////            rootVisualElement.Unbind();
////            if (_textField!=null)
////            {
////                _textField.value = "";
////            }
////        }
//    }
//}
//
