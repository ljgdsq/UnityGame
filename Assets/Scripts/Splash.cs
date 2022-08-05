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
    [SerializeField]
    private FadeAnimation _animation;
    // Start is called before the first frame update
    void Start()
    {
        _animation.OnComplete = () =>
        {
            SystemManager.Instance.ShowConnectWindow();

            DotweenEx.DoTime(1).OnComplete(() =>
            {
                SystemManager.Instance.HideConnectWindow();
            });
//            WebApiManager.Instance.Post(new HelloProtocol("hello"));
            DLCManager.Instance.CheckVersion();
            
    
        };
    }


    private void CheckVersion()
    {
        
    }
   
}
