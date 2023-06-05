using System.Collections;
using System.Collections.Generic;
using Loxodon.Framework.Observables;
using UnityEngine;
using UnityEngine.UI;
#if UNITY_EDITOR
    using UnityEditor;
#endif



public class MVCDemo : MonoBehaviour
{
    private void Start()
    {

        PlayerView view = FindObjectOfType<PlayerView>();
        
        PlayerController c=new PlayerController(view,new PlayerModel(0,""));
        c.Init();
     
    }
}

#if UNITY_EDITOR


#endif
