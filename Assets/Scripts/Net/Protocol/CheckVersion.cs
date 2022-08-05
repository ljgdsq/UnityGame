using System;
using Net.Protocol;
using UnityEngine;

namespace Net.Packet
{
    public class CheckVersion: ProtocolBase
    {
        public CheckVersion() : base(new CheckVersionReq(), new CheckVersionResp())
        {
        }

        class CheckVersionReq : Request
        {
            public override string GetURL()
            {
                return "/check_version";
            }
        }

        class CheckVersionResp : Response
        {
            public bool HasNew;
            public bool ForceUpdate;
            public string NewVer;
            public string DownloadURL;
        }

        public bool HasNew => ((CheckVersionResp) response).HasNew;
        public bool ForceUpdate => ((CheckVersionResp) response).ForceUpdate;
        public string NewVer => ((CheckVersionResp) response).NewVer;
        public string DownloadURL => ((CheckVersionResp) response).DownloadURL;

        public override void OnSuccess(Response value)
        {
            if (value is CheckVersionResp result)
            {
                this.response = result;
                if (result.HasNew)
                {
                    Debug.Log("new version :"+result.NewVer);
                }
            }
        }
    }
}