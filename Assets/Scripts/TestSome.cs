using System;
using UniRx;
using UnityEngine;
using UnityEngine.SceneManagement;

public class TestSome :  MonoBehaviour
{
    private bool _isStart = false;
    private void Start()
    {
        Debug.developerConsoleVisible = false;
        _isStart = true;

        Observable.IntervalFrame(60).TakeUntilDisable(this).Subscribe(x => Debug.Log(x), () =>
        {
            Debug.Log("complete!");
            
        });
        
        Observable.EveryUpdate().Where(_ => Input.GetMouseButtonDown(0)).Select((t1, t2) =>
        {
            
            Debug.Log(t1);
            Debug.Log(t2);
            return t1+t2;
        }).Subscribe((t) =>
        {
            Debug.Log(t);
        });
       var leftEvent= Observable.EveryUpdate().Where(_ => Input.GetMouseButtonDown(0)).Select(_ => true);
        var rightEvent=Observable.EveryUpdate().Where(_ => Input.GetMouseButtonUp(0)).Select(_ => false);
        var mouseUp = Observable.Merge(leftEvent,rightEvent);
        var reactiveCommand = new ReactiveCommand(mouseUp,false);
        reactiveCommand.Subscribe(x => { Debug.Log(x); });
//        Observable.EveryUpdate().Subscribe(t => { reactiveCommand.Execute(); }).AddTo(this);

    }


    private void Update()
    {
        if (_isStart)
        {
            

        }
    }
}