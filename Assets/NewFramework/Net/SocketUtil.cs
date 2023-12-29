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
}