using System.Collections.Generic;
using System.Reflection;
using UnityEngine;

namespace Util
{
    public static class DeviceInfo
    {
        public static Dictionary<string, string>  GetAllInfo()
        {
            var dic = new Dictionary<string, string>();
            var propertyInfos = typeof(SystemInfo).GetProperties(BindingFlags.Public | BindingFlags.Static);
            foreach (var propertyInfo in propertyInfos)
            {
                var name = propertyInfo.Name;
                var value = propertyInfo.GetValue(null);
                dic.Add(name, value.ToString());
            }

            return dic;
        }
    }
}