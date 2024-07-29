using System;
using System.Collections;
using System.Collections.Generic;
using SFramework.Manager;
using SFramework.Update;
using UnityEngine;
using YooAsset;

public class Boot : MonoBehaviour
{
    public EPlayMode PlayMode = EPlayMode.EditorSimulateMode;
    private void Awake()
    {
        GameManager.Instance.Behaviour = this;
        Debug.Log($"资源系统运行模式：{PlayMode}");
        Application.targetFrameRate = 60;
        Application.runInBackground = true;
        DontDestroyOnLoad(this.gameObject);
    }

    IEnumerator Start()
    {
        YooAssets.Initialize();
        yield return null;

        //show boot screen


        PatchOperation operation = new PatchOperation("DefaultPackage", PlayMode);
        YooAssets.StartOperation(operation);
        yield return operation;

        
        Debug.Log("PatchOperation success!");
        var gamePackage = YooAssets.GetPackage("DefaultPackage");
        YooAssets.SetDefaultPackage(gamePackage);
        
        Debug.Log("go main");
    }
    
}
