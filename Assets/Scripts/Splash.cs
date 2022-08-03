using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Splash : MonoBehaviour
{
    [SerializeField]
    private FadeAnimation _animation;
    // Start is called before the first frame update
    void Start()
    {
        _animation.OnComplete = () => { };
    }


    private void CheckVersion()
    {
        
    }
   
}
