#if UNITY_EDITOR
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class GizmoEx 
{

  //  [DrawGizmo(GizmoType.Selected)]
    private static void MyDrawGizmo(MeshRenderer meshRenderer, GizmoType gizmoType)
    {

        var color = Gizmos.color;
        Gizmos.color=Color.green;
        
        Gizmos.DrawWireCube(meshRenderer.transform.position,Vector3.one*1.5f);

        Gizmos.color = color;

    }
    
    
    
    
}

#endif

