using System;
using System.IO;
using System.Net.Sockets;
using UnityEngine;
using Logger = Base.Logger;

namespace Net
{
    public class SocketClient
    {
        private string _host;
        private int _port;

        private TcpClient _client;
        private NetworkStream _networkStream;
        private const int BUFFER_LENGTH = 1024 * 16;
        private byte[] _buffer = new byte[BUFFER_LENGTH];

        private bool _connected = false;

        public void ConnectServer(string host, int port)
        {
            if (_client != null)
            {
                _client.Close();
                _client = null;
            }

            this._host = host;
            this._port = port;

            _client = new TcpClient();
            _client.SendTimeout = 1000;
            _client.ReceiveTimeout = 1000;
            _client.NoDelay = true;

            try
            {
                _client.BeginConnect(host, port, OnConnect, null);
            }
            catch (Exception e)
            {
                OnDisconnected(e.Message);
            }
        }


        private void OnConnect(IAsyncResult ir)
        {
            _client.EndConnect(ir);
            _networkStream = _client.GetStream();
            _networkStream.BeginRead(_buffer, 0, BUFFER_LENGTH, OnReadEnd, null);
        }

        private void OnReadEnd(IAsyncResult ir)
        {
            if (!_connected)
            {
                return;
            }

            try
            {
                int bytesRead = 0;

                {
                    bytesRead = _networkStream.EndRead(ir);
                }

                if (bytesRead < 1)
                {
                    OnDisconnected("bytesRead < 1");
                    return;
                }

                OnReceive(_buffer, bytesRead);

                {
                    Array.Clear(_buffer, 0, _buffer.Length);
                    _networkStream.BeginRead(_buffer, 0, BUFFER_LENGTH, OnReadEnd, null);
                }
            }
            catch (Exception e)
            {
                if (e.GetType() == typeof(ObjectDisposedException))
                    return;
                OnDisconnected("OnRead failed ! : " + e.Message);
            }
        }

        protected virtual void OnReceive(byte[] buffer, int len)
        {
        }

        public void SendMessage(byte[] buffer, int length = 0)
        {
            if (length == 0)
            {
                length = buffer.Length;
            }

            _networkStream.BeginWrite(buffer, 0, length, OnWriteEnd, null);
        }


        private void OnWriteEnd(IAsyncResult ir)
        {
            if (!_connected)
            {
                return;
            }

            _networkStream.EndWrite(ir);
        }

        private void OnDisconnected(string msg)
        {
            Logger.LogError("OnDisconnected:" + msg);
            Close();
        }

        private void Close()
        {
            _connected = false;
            _networkStream?.Close();
            if (_client != null)
            {
                if (_client.Connected)
                    _client.Close();
            }
        }
    }
}