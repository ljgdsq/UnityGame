using System;
using System.Linq;
using System.Reflection;
using UnityEngine;
using Util;

namespace Save
{



    public class SaveAttribute : Attribute
    {
        public string Name { get; }

        public SaveAttribute(string name="")
        {
            Name = name;
        }
    }

    
    public class TransientAttribute:Attribute{}

    public class SaveDataBase
    {
        
    }
    
    public class SaveData
    {
        private bool _isLoad = false;
        public void Load()
        {
       
            if (_isLoad) return;
            _isLoad = true;

            var allTypes = AssemblyUtil.GetAllTypes(typeof(SaveAttribute));
            foreach (Type type in allTypes)
            {
                var instance = Activator.CreateInstance(type);
                var propertyInfos = type.GetProperties();
                foreach (var propertyInfo in propertyInfos)
                {
                    if (!propertyInfo.IsDefined(typeof(TransientAttribute)))
                    {
                    }
                }
            }
            
        }

        public void Save()
        {
            
        }

        public T Get<T>() where T : new()
        {
            return new T();
        }

        public void Save<T>(T obj)
        {
            
        }


    }
}