using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GizemoExample : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnDrawGizmos()
    {
        var color = Gizmos.color;

        Gizmos.color=Color.magenta;
        Gizmos.DrawCube(transform.position,Vector3.one);
        Gizmos.color = color;

    }


    private void OnDrawGizmosSelected()
    {
        Gizmos.DrawWireSphere(transform.position,2);
    }
    
    
    
}
