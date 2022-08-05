using UnityEngine;

namespace Net.Protocol
{
    public class HelloProtocol : ProtocolBase
    {
        public HelloProtocol(string msg) : base(new HomeReq(msg), new HomeResp())
        {
        }

        class HomeReq : Request
        {
            public HomeReq(string msg)
            {
                this.Msg = msg;
            }

            public override string GetURL()
            {
                return "/hello";
            }

            public string Msg { get; set; }
        }

        class HomeResp : Response
        {
            public string ReplyMsg { get; set; }
        }

        public override void OnSuccess(Response resp)
        {
            if (resp is HomeResp result)
            {
                Debug.Log(result.ReplyMsg);
            }
        }
    }
}