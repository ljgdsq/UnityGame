using System;
using System.Text;
using Newtonsoft.Json;

namespace Net.Protocol
{
    public abstract class Request
    {
        public abstract string GetURL();
    }

    public abstract class Response
    {
    }

    [Serializable]
    public abstract class ProtocolBase
    {
        protected Request request;

        protected Response response;

        public ProtocolBase(Request request, Response response)
        {
            this.request = request;
            this.response = response;
        }

        public string GetURL()
        {
            return request.GetURL();
        }

        public byte[] GetData()
        {
            var json = JsonConvert.SerializeObject(request);
            return Encoding.UTF8.GetBytes(json);
        }

        public void OnGetResult(string value)
        {
            var resp = JsonConvert.DeserializeObject(value, response.GetType());
            OnSuccess((Response) resp);
        }

        public virtual void OnSuccess(Response value)
        {
        }
    }
}