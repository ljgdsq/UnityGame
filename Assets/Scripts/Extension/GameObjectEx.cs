using UnityEngine;

namespace Extension
{
    public static class GameObjectEx
    {
        public static T RequireComponent<T>(this GameObject go) where T : Component
        {
            var component = go.GetComponent<T>();
            if (component==null)
            {
                component=go.AddComponent<T>();
            }
            return component;
        }
    }
}