using System;

namespace Save
{
    public interface IArchive
    {
        void Serialize(string name, object obj);
        object DeSerialize(string name,Type type);
    }
}