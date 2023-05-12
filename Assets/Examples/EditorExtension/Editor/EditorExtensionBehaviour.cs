using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

[AddComponentMenu("Test/EditorExtensionBehaviour")]
[DisallowMultipleComponent]
[ExecuteInEditMode]
[HelpURL("https://github.com/ljgdsq/UnityGame")]
[RequireComponent(typeof(Canvas))]
public class EditorExtensionBehaviour : MonoBehaviour
{
    private void Awake()
    {
        Debug.Log("Awake");
    }

    private void Start()
    {
        Debug.Log("Start");
    }


    private void Update()
    {
        Debug.Log("Update");
    }

    [ContextMenu("DoSome")]
    void DoSome()
    {
        Debug.Log("do some!");
    }



    [ContextMenuItem("Reset",nameof(ResetDesc))]
    [Multiline(6)]
    public string desc;

    
    [TextArea]
    public string desc2;
    void ResetDesc()
    {
        desc = "";
    }


//    [GUITarget(1,0)]
    [GUITarget(0,1,2)] 
    private void OnGUI()
    {
        GUI.Label(new Rect(10,20,300,80), "OnGUI Text");
    }


    [Delayed]
    public string DelayAttrtTest;


    [Header("this is header")]
    public string Header;


    [HideInInspector]
    public int aaa;
    
    [Space(20)]
    [Range(0,100)]
    [Tooltip("age from 0-100")]
    public int age;


    public Color MyColor;

//    [RuntimeInitializeOnLoadMethod]
    static void OnRuntimeInit()
    {
        Debug.LogError("RuntimeInitializeOnLoadMethod OnRuntimeInit");
    }


    [DrawGizmo(GizmoType.InSelectionHierarchy)]
    static void MyGizmo(Camera camera, GizmoType gizmoType)
    {
        
        
        Gizmos.color=Color.red;
        Gizmos.DrawWireSphere(camera.transform.position,1);
    }
    
    
}
