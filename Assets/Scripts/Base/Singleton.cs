using System.Collections;

namespace Base
{
    public class Singleton<T> where T : new()
    {
        private static T _instance;

        public static T GetInstance()
        {
            if (_instance==null)
            {
                _instance =new T();
            }

            return _instance;
        }

        public static T Instance => GetInstance();

   

        public virtual void  Init()
        {
        }
    }
}