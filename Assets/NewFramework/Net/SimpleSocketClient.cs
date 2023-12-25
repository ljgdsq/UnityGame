using System;

namespace Net
{

    public struct MsgHead
    {
        public UInt16 length;
        public UInt16 cmd;
        public byte flag;
    }
    
    public class SimpleSocketClient :SocketClient
    {
        
        protected override void OnReceive(byte[] buffer, int len)
        {
            
        }
    }
}