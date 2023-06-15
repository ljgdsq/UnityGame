
#if UNITY_EDITOR
using UnityEditor;
using UnityEngine;
public class GameViewSyncToSceneView : MonoBehaviour
{
    private void Update()
    {
        Camera mainCamera=Camera.main;
        SceneView sceneView = SceneView.lastActiveSceneView;
        if (sceneView)
        {
            sceneView.cameraSettings.nearClip = mainCamera.nearClipPlane;
            sceneView.cameraSettings.fieldOfView = mainCamera.fieldOfView;
            sceneView.pivot = mainCamera.transform.position +
                              mainCamera.transform.forward * sceneView.cameraDistance;
            sceneView.rotation = mainCamera.transform.rotation;
        }
    }
    
}
#endif
