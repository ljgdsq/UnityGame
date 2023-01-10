// Simple Wizard that clones an object several times.

#if UNITY_EDITOR

using UnityEngine;
using UnityEditor;
using System.Collections;

public class CloneObjects : ScriptableWizard
{
    public GameObject objectToCopy = null;
    public int numberOfCopies = 2;
    [MenuItem("Example/Clone objects")]
    static void CreateWindow()
    {
        // Creates the wizard for display
        ScriptableWizard.DisplayWizard("Clone an object.", typeof(CloneObjects), "Clone!");
    }

    void OnWizardUpdate()
    {
        helpString = "Clones an object a number of times and move the cloned objects to the origin";
        if (!objectToCopy)
        {
            errorString = "Please assign an object";
            isValid = false;
        }
        else
        {
            errorString = "";
            isValid = true;
        }
    }

    void OnWizardCreate()
    {
        for (int i = 0; i < numberOfCopies; i++)
            Instantiate(objectToCopy, Vector3.zero, Quaternion.identity);
    }
}

#endif
