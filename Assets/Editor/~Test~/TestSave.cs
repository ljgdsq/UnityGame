using System;
using System.Collections.Generic;
using Base;
using Extension;
using Save;
using UnityEditor;
using UnityEditor.Build.Pipeline;

namespace Test
{
    [Save]
    [Serializable]
    public class TestSave
    {
        public string Name;
        public int Level;
        public string Desc;
        [NonSerialized] public float Tag;
    }


    public class TestPlayer
    {
        public string Name;

        public List<string> NickNames;
    }

    public class TestSaveClass
    {
        [MenuItem("Test/TestSome")]
        public static void TestSome()
        {
            SaveGame saveGame = SaveGame.Instance;
            saveGame.LoadAll();

            var orCreate = saveGame.GetOrCreate<TestSave>(out bool isNew);
            if (isNew)
            {
            }

            orCreate.Level = 99;
            orCreate.Tag = 1000;
            saveGame.SaveAll();

//            saveGame.Save<TestSave>();
            var testPlayer = saveGame.GetOrCreate<TestPlayer>(out isNew);

            testPlayer.Name = "zzz";

            saveGame.Save<TestPlayer>();
//            saveGame.SaveAll();
        }


        [MenuItem("Test/TestBinaryArchive")]
        public static void TestBinaryArchive()
        {
            var archive = new BinaryArchive();
//            var obj = new TestSave();
//            obj.Level = 999;
//            obj.Name = "aaa";
//            obj.Tag = 1001;
//            archive.Serialize("TestSave",obj);

            var deSerialize = archive.DeSerialize("TestSave", null);

            Logger.Log(deSerialize.ToString());
        }

        [MenuItem("Test/TestEncrypt")]
        public static void TestEncrypt()
        {
            var str = "Hello";
            var utf8Bytes = str.GetUTF8Bytes();
            var utf8String = utf8Bytes.GetUTF8String();

            var encrypt = EncryptHelper.Encrypt("Hello world!".GetUTF8Bytes(),"tt");
            var data = EncryptHelper.Decrypt(encrypt,"tt");
            Logger.Log(data.GetUTF8String());


            var s = EncryptHelper.Encrypt("Hello world!","tt");
            var bytes = s.GetUTF8Bytes();
            var decrypt = EncryptHelper.Decrypt(s,"tt");
            var utf8Bytes1 = decrypt.GetUTF8Bytes();

            Logger.Log(decrypt);
        }

        [MenuItem("Test/TestEncryptJson")]

        public static void TestEncryptJson()
        {
            var playerPrefsJsonArchive = new PlayerPrefsJsonArchive();
            var obj = new TestSave();
            obj.Level = 999;
            obj.Name = "aaa";
            obj.Tag = 1001;
            playerPrefsJsonArchive.Serialize("TestSave",obj);
            var deSerialize = playerPrefsJsonArchive.DeSerialize("TestSave",typeof(TestSave));
            Logger.Log(deSerialize.ToString());
            
        }
        
    }
}