using System;
using System.IO;
using System.Security.Cryptography;

namespace Util
{
    public class MD5Util
    {
        public static string CalcMd5(string filePath)  
        {             
            try  
            {  
                FileStream fs = new FileStream(filePath, FileMode.Open);  
                int len = (int)fs.Length;  
                byte[] data = new byte[len];  
                fs.Read(data, 0, len);  
                fs.Close();  
                MD5 md5 = new MD5CryptoServiceProvider();  
                byte[] result = md5.ComputeHash(data);  
                string fileMD5 = "";  
                foreach (byte b in result)  
                {  
                    fileMD5 += Convert.ToString(b, 16);  
                }  
                return fileMD5;     
            }  
            catch (FileNotFoundException e)  
            {  
                Console.WriteLine(e.Message);  
                return "";  
            }                                   
        }
    }
}