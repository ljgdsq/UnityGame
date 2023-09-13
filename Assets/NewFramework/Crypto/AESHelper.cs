using System;
using System.Linq;
using System.Security.Cryptography;
using System.Text;

namespace Crypto
{
    public class AESHelper
    {
        private static string KEY = "0987654321@****@0123456789abcedf";
        private static string IV = "1234567890123456";
        private static string KeySalt = "KeySalt";
        private static ICryptoTransform encryptor;
        private static ICryptoTransform decryptor;
        private static bool _inited = false;

        public static void SetKey(string key, string iv)
        {
            _inited = false;
            KEY = key;
            IV = iv;
            Init();
        }

        public static void Init()
        {
            if (_inited) return;
            RijndaelManaged rijndael = new RijndaelManaged();
            var sha256Managed = new SHA256Managed();
            var key = sha256Managed.ComputeHash(Encoding.UTF8.GetBytes(KEY).Concat(Encoding.UTF8.GetBytes(KeySalt))
                .ToArray());
            rijndael.Key = key;
            rijndael.IV = Encoding.UTF8.GetBytes(IV);
            rijndael.Mode = CipherMode.ECB;
            rijndael.Padding = PaddingMode.PKCS7;
            encryptor = rijndael.CreateEncryptor();
            decryptor = rijndael.CreateDecryptor();
        }


        public static byte[] Encrypt(byte[] data)
        {
            Init();
            return encryptor.TransformFinalBlock(data, 0, data.Length);
        }

        public static byte[] Decrypt(byte[] data)
        {
            Init();
            return decryptor.TransformFinalBlock(data, 0, data.Length);
        }

        public static byte[] Encrypt(string data)
        {
            return Encrypt(Encoding.UTF8.GetBytes(data));
        }

        public static string EncryptToBase64(string data)
        {
            var encrypt = Encrypt(data);
            return Convert.ToBase64String(encrypt);
        }


        public static byte[] Decrypt(string data)
        {
            return Decrypt(Convert.FromBase64String(data));
        }

        public static string DecryptFromBase64(string data)
        {
            var decrypt = Decrypt(data);
            return Encoding.UTF8.GetString(decrypt);
        }
    }
}