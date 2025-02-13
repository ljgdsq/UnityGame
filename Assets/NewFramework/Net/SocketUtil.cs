using System;
using System.Net;
using System.Net.Sockets;

namespace Net
{


    public class SocketUtil
    {
        private static readonly byte[] EmptyPack = new byte[1];
        
        /// <summary>
        /// see https://learn.microsoft.com/zh-cn/dotnet/api/system.net.sockets.socket.connected
        /// </summary>
        /// <param name="client"></param>
        /// <returns></returns>
        public static bool IsSocketConnected(Socket client)
        {
            bool blockingState = client.Blocking;
            bool connectState = true;
            try
            {
                client.Blocking = false;
                client.Send(EmptyPack, 0, 0);
            }
            catch (SocketException e)
            {
                // 10035 == WSAEWOULDBLOCK
                if (e.NativeErrorCode.Equals(10035))
                {
                    connectState = true;
                    // Console.WriteLine("Still Connected, but the Send would block");
                }
                else
                {
                    connectState = false;
                    // Console.WriteLine("Disconnected: error code {0}!", e.NativeErrorCode);
                }
            }
            
            client.Blocking = blockingState;

            return connectState;
        }
    }
    
    public class NetworkProtocol 
    {
        public static byte[] PackMessage(ushort msgId, byte[] payload) 
        {
            // 1. 准备长度和消息号
            ushort length = (ushort)(payload.Length + 2);
        
            // 2. 转换为网络字节序（大端）
            ushort networkLength = IPAddress.HostToNetworkOrder((short)length);
            ushort networkMsgId = IPAddress.HostToNetworkOrder((short)msgId);
        
            // 3. 打包消息
            byte[] message = new byte[length + 2];
            Buffer.BlockCopy(BitConverter.GetBytes(networkLength), 0, message, 0, 2);
            Buffer.BlockCopy(BitConverter.GetBytes(networkMsgId), 0, message, 2, 2);
            Buffer.BlockCopy(payload, 0, message, 4, payload.Length);
        
            return message;
        }

        public static (ushort msgId, byte[] payload) UnpackMessage(byte[] data) 
        {
            // 1. 读取网络字节序的数据
            short networkLength = BitConverter.ToInt16(data, 0);
            short networkMsgId = BitConverter.ToInt16(data, 2);
        
            // 2. 转换回主机字节序
            ushort length = (ushort)IPAddress.NetworkToHostOrder(networkLength);
            ushort msgId = (ushort)IPAddress.NetworkToHostOrder(networkMsgId);
        
            // 3. 解析消息体
            byte[] payload = new byte[length - 2];
            Buffer.BlockCopy(data, 4, payload, 0, payload.Length);
        
            return (msgId, payload);
        }
    }
}