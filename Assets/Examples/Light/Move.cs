using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Move : MonoBehaviour
{

    public float speed = 1.0f;

    private float oldPosX = 0;
    // Start is called before the first frame update
    void Start()
    {
        oldPosX = transform.position.x;
    }

    // Update is called once per frame
    void Update()
    {
        
        var x=oldPosX + Mathf.Sin(Time.frameCount*speed*0.001f);
        transform.position=new Vector3(x,transform.position.y,transform.position.z);
    }
}
