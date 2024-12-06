using System;
using UnityEngine;

namespace Goose.UIFramework {
    #region Enums
    public enum UIType 
    {
        Window,     // 主要界面
        Dialog      // 弹出框
    }

    public enum UISize 
    {
        Custom,     // 自定义大小
        FullScreen, // 全屏
    }

    public enum UILayer 
    {
        Background = 0,
        Normal = 1000,
        Dialog = 2000,
        Top = 3000
    }

    public enum UIState 
    {
        None,
        Loading,
        Loaded,
        Showing,
        Shown,
        Hiding,
        Hidden,
        Destroyed
    }
    #endregion

    #region Attributes
    [AttributeUsage(AttributeTargets.Class)]
    public class UIConfigAttribute : Attribute 
    {
        public string ResourcePath { get; }
        public UIType Type { get; }
        public UISize Size { get; }
        public UILayer Layer { get; }
        public bool IsPersistent { get; }
        
        public UIConfigAttribute(
            string resourcePath,
            UIType type = UIType.Window,
            UISize size = UISize.Custom,
            UILayer layer = UILayer.Normal,
            bool isPersistent = false)
        {
            ResourcePath = resourcePath;
            Type = type;
            Size = size;
            Layer = layer;
            IsPersistent = isPersistent;
        }
    }
    #endregion

}