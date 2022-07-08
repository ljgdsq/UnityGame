using System;
using System.Collections.Generic;
using System.Reflection;

namespace Util
{
    public static class AssemblyUtil
    {
        public static List<Type> GetAllTypes(Type attribute)
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
        
        
    }
}