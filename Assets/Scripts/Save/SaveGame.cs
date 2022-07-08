using System;
using System.Collections.Generic;
using System.Diagnostics;
using Base;
using UnityEngine;
using Util;
using Debug = UnityEngine.Debug;
using Logger = Base.Logger;


namespace Save
{
    public class SaveAttribute : Attribute
    {
        public string Name { get; }

        public SaveAttribute(string name = "")
        {
            Name = name;
        }
    }

    public class SaveGame : Singleton<SaveGame>
    {
        private Dictionary<Type, object> _saves = new Dictionary<Type, object>();
        private bool _isLoad ;

        private IArchive _archive;

        public void SetArchive(IArchive archive)
        {
            _archive = archive;
        }

        public void LoadAll()
        {
            if (_isLoad) return;
            _isLoad = true;

            var allTypes = ReflectionUtil.GetAllTypesHasDefineType(typeof(SaveAttribute));
            foreach (Type type in allTypes)
            {
                var obj = _archive.DeSerialize(type.Name, type);
                if (obj != null)
                {
                    Logger.Log($"load game save {type.Name} ");
                    _saves.Add(type, obj);
                }
            }
        }

        public void SaveAll()
        {
            foreach (var keyValuePair in _saves)
            {
                SaveObject(keyValuePair.Value);
            }
        }

        public T GetOrCreate<T>(out bool isNew) where T : new()
        {
            TypeCheck<T>();
            if (_saves.ContainsKey(typeof(T)))
            {
                isNew = false;
                return (T) _saves[typeof(T)];
            }
            else
            {
                isNew = true;
                var obj = new T();
                _saves.Add(typeof(T), obj);
                return obj;
            }
        }

        public void Save<T>() where T : new()
        {
            TypeCheck<T>();
            if (_saves.ContainsKey(typeof(T)))
            {
                var obj = _saves[typeof(T)];
                SaveObject(obj);
            }
        }

        private void SaveObject<T>(T obj)
        {
            _archive.Serialize(typeof(T).Name, obj);
        }

        public override void Init()
        {
            LoadAll();
        }

        [Conditional("DEBUG")]
        private void TypeCheck<T>()
        {
            if (!ReflectionUtil.IsDefineAttribute<T>(typeof(SaveAttribute)))
            {
                Logger.LogError($"this type {typeof(T)} should define a SaveAttribute!");
            }
        }
    }
}