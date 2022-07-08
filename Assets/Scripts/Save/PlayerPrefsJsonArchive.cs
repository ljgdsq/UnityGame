using System;
using Base;
using UnityEngine;

namespace Save
{
    public class PlayerPrefsJsonArchive:IArchive
    {
        public void Serialize(string name, object obj)
        {
            var json = JsonUtility.ToJson(obj);

#if USE_ENCRYPT
            json = EncryptHelper.Encrypt(json, name);
#endif
            
            PlayerPrefs.SetString(obj.GetType().Name, json);
        }

        public object DeSerialize(string name,Type type)
        {
            var json=PlayerPrefs.GetString(name, "");
#if USE_ENCRYPT
           json = EncryptHelper.Decrypt(json, name);

#endif
            return JsonUtility.FromJson(json,type);
        }
    }
}