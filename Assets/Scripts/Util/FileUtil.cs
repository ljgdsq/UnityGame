using System.IO;

namespace Util
{
    public static class FileUtil
    {
        public static void CopyFolder(string src, string dst,bool clearDst=true)
        {
            var exists = Directory.Exists(dst);
            if (exists)
            {
                Directory.Delete(dst,true);
            }

            Directory.CreateDirectory(dst);

     
        }
    }
}