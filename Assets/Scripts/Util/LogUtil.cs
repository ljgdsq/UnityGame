using System;
using System.Diagnostics;

namespace Base
{
    public class Logger
    {
        [Conditional("DEBUG")]
        public static void  Log(String msg)
        {
            UnityEngine.Debug.Log(msg);
        } 
        
        [Conditional("DEBUG")]
        public static void  LogWarning(String msg)
        {
            UnityEngine.Debug.LogWarning(msg);
        } 
        
        [Conditional("DEBUG")]
        public static void  LogError(String msg)
        {
            UnityEngine.Debug.LogError(msg);
        } 
        
    }
}