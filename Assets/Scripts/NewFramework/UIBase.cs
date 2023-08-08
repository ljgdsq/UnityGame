using UnityEngine;

namespace GameFramework
{
    
    public enum UILayer
    {
        Background = 0,
        Default = 1,
        Popup = 2,
        Top = 3,
    }

    public enum UIType
    {
        /// <summary>
        ///  when open it will close the last window,
        ///  when close it will open the last window (if last window exists) 
        /// </summary>
        Window,
        Dialog,
    }

    public enum UIDestroyMode
    {
        Never,
        SceneChange,
        Hide,
        WindowChange,
    }

    public enum UIHideMode
    {
        ActiveFalse,
        ChangeLayer,
    }
    
    public class UIBase:MonoBehaviour
    {

        
    }
}