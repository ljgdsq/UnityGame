using System;
using System.IO;
using System.Linq;

namespace NewFramework.Extension
{
    public static class StringExtension
    {
        public static string NormalizePath(this string path)
        {
            if (Path.DirectorySeparatorChar=='\\')
            {
                return path.Replace('/', Path.DirectorySeparatorChar);
            }
            return path.Replace('\\', Path.DirectorySeparatorChar);
        }
        
        public static string ConvertSeparatorsToWindows(this string path)
        {
            return path.Replace('/', '\\');
        }

        public static string ConvertSeparatorsToUnity(this string path)
        {
            return path.Replace('\\', '/');
        }
        
        public static string TrimTrailingSlashes(this string path)
        {
            return path.TrimEnd(new[] { '/', '\\' });
        }
        
        public static string GetPathRelativeToProjectDirectory(this string filePath)
        {
            return String.Join(separator: "/",
                value: filePath.ConvertSeparatorsToUnity().TrimTrailingSlashes().Split('/').SkipWhile(s => s != "assets" && s != "Assets").ToArray());
        }
    }
}