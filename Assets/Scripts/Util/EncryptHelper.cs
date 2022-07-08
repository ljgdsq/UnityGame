using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;

namespace Base
{
    public static class EncryptHelper
    {
        private static string KEY = "0987654321@****@0123456789abcedf";
        private static string IV = "1234567890123456";


        public static byte[] Encrypt(byte[] data, string keySalt="")
        {
            RijndaelManaged rijndael = new RijndaelManaged();

            var sha256Managed = new SHA256Managed();
            var key=sha256Managed.ComputeHash(Encoding.UTF8.GetBytes(KEY).Concat(Encoding.UTF8.GetBytes(keySalt)).ToArray());
            rijndael.Key = key;
            rijndael.IV = Encoding.UTF8.GetBytes(IV);

            ICryptoTransform encryptor = rijndael.CreateEncryptor();

            return encryptor.TransformFinalBlock(data, 0, data.Length);
        }

        public static byte[] Decrypt(byte[] data, string keySalt="")
        {
            RijndaelManaged rijndael = new RijndaelManaged();
            var sha256Managed = new SHA256Managed();
            var key=sha256Managed.ComputeHash(Encoding.UTF8.GetBytes(KEY).Concat(Encoding.UTF8.GetBytes(keySalt)).ToArray());
            rijndael.Key = key;
            rijndael.IV = Encoding.UTF8.GetBytes(IV);

            ICryptoTransform decryptor = rijndael.CreateDecryptor();

            return decryptor.TransformFinalBlock(data, 0, data.Length);
        }

        public static string Encrypt(string data, string keySalt = "")
        {
            var encrypt = Encrypt(Encoding.UTF8.GetBytes(data),keySalt);
            return Convert.ToBase64String(encrypt);
        }

        public static string Decrypt(string data, string keySalt = "")
        {
            var decrypt = Decrypt(Convert.FromBase64String(data),keySalt);
            return Encoding.UTF8.GetString(decrypt);
        }

    }
}