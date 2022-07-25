using System;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace Save
{
    public class BinaryArchive : IArchive
    {
        public void Serialize(string name, object obj)
        {
            var path = GameApplication.GetWriteablePath() + "/" + name + ".bin";
            using (FileStream fs = File.Open(path, FileMode.OpenOrCreate))
            {
                BinaryFormatter formatter = new BinaryFormatter();
                formatter.Serialize(fs, obj);
            }
        }

        public object DeSerialize(string name, Type type)
        {
            var path = GameApplication.GetWriteablePath() + "/" + name + ".bin";
            if (File.Exists(path) )
            {
                var info=new FileInfo(path);
                if (info.Length>0)
                {
                    using (FileStream fs = File.Open(path, FileMode.Open))
                    {
                        BinaryFormatter formatter = new BinaryFormatter();
                        return formatter.Deserialize(fs);
                    }
                }
            }

            return null;
        }
    }
}