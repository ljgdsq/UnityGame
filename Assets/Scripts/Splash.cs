using System.Collections;
using System.Collections.Generic;
using DG.Tweening;
using Extension;
using Manager;
using Net;
using Net.Packet;
using Net.Protocol;
using UnityEngine;

public class Splash : MonoBehaviour
{

    void Start()
    {
        DotweenEx.DoTime(9.0f).OnComplete(() =>
        {
            SystemManager.Instance.ShowConnectWindow();

            DotweenEx.DoTime(1).OnComplete(() => { SystemManager.Instance.HideConnectWindow(); });
            DLCManager.Instance.CheckVersion();
        });
    }
   
}
