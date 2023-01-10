#if UNITY_EDITOR
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class ScriptableWizardTest2 : ScriptableWizard
{

    private string msg;

    public string inpoutSome;
    [MenuItem("MyWindow/ScriptableWizardTest2")]
    private static void ShowWindow()
    {
        ScriptableWizard.DisplayWizard<ScriptableWizardTest2>("this is title", "ok", "no");
    }

    private void OnWizardUpdate()
    {
        Debug.Log("OnWizardUpdate");

        if (string.IsNullOrEmpty(msg))
        {
            errorString = "input text";
            helpString = "";
        }
        else
        {
            errorString = "";
            helpString = "help str";
        }
        
    }


    protected override bool DrawWizardGUI()
    {

       msg= GUILayout.TextArea(msg);
        
        return base.DrawWizardGUI();
    }

    private void OnWizardCreate()
    {
        Debug.Log("OnWizardCreate");

    }

    private void OnWizardOtherButton()
    {
        Debug.Log("OnWizardOtherButton");
    }
}






#endif

