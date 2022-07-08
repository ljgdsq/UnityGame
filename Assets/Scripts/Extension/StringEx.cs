using System.Text;

namespace Extension
{
    public static class StringEx
    {
        public static byte[] GetUTF8Bytes(this string msg)
        {
            return Encoding.UTF8.GetBytes(msg);
        }


        public static string GetUTF8String(this byte[] data)
        {
            return Encoding.UTF8.GetString(data);
        }
    }
}