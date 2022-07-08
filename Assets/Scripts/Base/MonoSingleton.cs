using System.Collections;
using UnityEngine;

namespace Base
{
    public abstract class MonoSingleton<T> : MonoBehaviour where T : MonoBehaviour
    {
        private static T _instance;

        public static T Instance => GetInstance();

        public static T GetInstance()
        {
            if (_instance == null)
            {
                var objects = FindObjectsOfType(typeof(T));
                if (objects.Length >= 1)
                {
                    if (objects.Length >= 2)
                    {
                        Logger.LogError($"find {objects.Length} singleton !!! please fix this.");
                    }

                    _instance = (T) objects[0];
                }
                else
                {
                    var obj = new GameObject();
                    _instance = obj.AddComponent<T>();
                    obj.name = typeof(T).Name;
                    DontDestroyOnLoad(obj);
                }
            }

            return _instance;
        }

        public virtual void Init()
        {
        }
    }
}