using System.IO;

namespace NewFramework.Base
{
    public class MemoryBuffer
    {
        private const int InitSize = 4096;
        private MemoryStream _memoryStream;
        private BinaryReader _reader;
        private BinaryWriter _writer;

        public MemoryBuffer()
        {
            _memoryStream = new MemoryStream(InitSize);
            _writer = new BinaryWriter(_memoryStream);
            _reader = new BinaryReader(_memoryStream);
        }

        public void WriteInt(int value)
        {
            _writer.Write(value);
        }
        
    }
}