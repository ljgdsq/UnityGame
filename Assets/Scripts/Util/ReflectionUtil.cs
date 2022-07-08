using System;
using System.Collections.Generic;
using System.Reflection;

namespace Util
{
    public static class ReflectionUtil
    {
        public static List<Type> GetAllTypesHasDefineType(Type attribute)
        {
            List<Type> list=new List<Type>();
            var exportedTypes = Assembly.GetExecutingAssembly().GetExportedTypes();
            foreach (var exportedType in exportedTypes)
            {
                if (exportedType.IsDefined(attribute,true))
                {
                    list.Add(exportedType);
                }
            }
            return list;
        }

        public static bool IsDefineAttribute<T>(Type attribute,bool inheirt=true)
        {
            var type = typeof(T);
            if (type.IsDefined(attribute,inheirt))
            {
                return true;
            }
            return false;
        }
        
    }
}