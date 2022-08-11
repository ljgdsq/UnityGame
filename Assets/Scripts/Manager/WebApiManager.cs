using System.IO;
using System.Net;
using System.Text;
using Base;
using Net.Protocol;

namespace Manager
{
    public class WebApiManager:MonoSingleton<WebApiManager>
    {
        //todo use sub thead do it
        private readonly string host = "http://127.0.0.1:8080";

        public static string DownloadUrl;
        public static string NewVersion;
        
        public void Post(ProtocolBase protocol)
        {
            var url = host + protocol.GetURL();
            var webRequest = WebRequest.Create(url);
            webRequest.Method = "POST";
            webRequest.ContentType = "application/json;charset=utf-8";
            webRequest.ContentLength = protocol.GetData().Length;
            using (Stream stream=webRequest.GetRequestStream())
            {
                stream.Write(protocol.GetData(),0, (int) webRequest.ContentLength);
                stream.Flush();
            }
            var webResponse = webRequest.GetResponse();
            using (Stream stream=webResponse.GetResponseStream())
            {
                using (StreamReader reader=new StreamReader(stream,Encoding.UTF8))
                {
                    var resp = reader.ReadToEnd();
                    protocol.OnGetResult(resp);
                }
            }
        }

        public void Get(ProtocolBase protocol)
        {

#if UNITY_EDITOR
            if (StaticConfig.EnableOffline)
            {
                protocol.OnGetFakeResult();
                return;
            }       
#endif
      
            
            var url = host + protocol.GetURL();
            var webRequest = WebRequest.Create(url);
            webRequest.Method = "GET";
            webRequest.ContentType = "application/json;charset=utf-8";
            
            var webResponse = webRequest.GetResponse();
            using (Stream stream=webResponse.GetResponseStream())
            {
                using (StreamReader reader=new StreamReader(stream,Encoding.UTF8))
                {
                    var resp = reader.ReadToEnd();
                    protocol.OnGetResult(resp);
                }
            }
        }
    }
}