using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using Base;
using SAssetbundle;
using UnityEngine;

namespace Manager
{
    public class DownloadManager : MonoSingleton<DownloadManager>
    {
        //127.0.0.1/get_dlc
        //127.0.0.1/get_dlc?ver=1.0
        //127.0.0.1/get_dlc?ver=1.0&file="aaa"
        public string DownloadURL;

        private long _TotalNeedDownSize;
        private long _CurrentDownSize;

        public event Action<long, long> OnDownProgress;
        public event Action OnDownComplete;

        public void DownloadManifestTemp()
        {
            SystemManager.Instance.ShowConnectWindow();
            var file = "manifest.ab";
            var webRequest = WebRequest.Create(DownloadURL + "?ver=" + DLCManager.Instance.NewVer + "&file=" + file);
            webRequest.Method = "GET";
            webRequest.ContentType = "application/octet-stream";

            var webResponse = webRequest.GetResponse();

            using (Stream stream = webResponse.GetResponseStream())
            {
                var len = webResponse.ContentLength;
                byte[] buff = new byte[len];
                int off = 0;
                int redlen = 0;
                while (off < len && (redlen = stream.Read(buff, off, (int) len)) > 0)
                {
                    off = redlen;
                }

                bool needUpdate = true;
                var assetBundle = AssetBundle.LoadFromMemory(buff);
                var textAsset = assetBundle.LoadAsset<TextAsset>(BundleManifest.ManifestName);
                BundleManifest newManifest = BundleManifest.LoadFromJson(textAsset.text);
                var newBundleInfos = newManifest.GetBundleInfos();
                if (AssetBundleManager.Instance.BundleManifest == null)
                {
                    _TotalNeedDownSize = 0;
                    _CurrentDownSize = 0;
                    foreach (var bundleInfo in newBundleInfos)
                    {
//               
                        _TotalNeedDownSize += bundleInfo.Size;
                        BeginDownload(bundleInfo.Name, bundleInfo.Size);
                    }
                }
                else
                {
                    var oldBundleInfos = AssetBundleManager.Instance.BundleManifest.GetBundleInfos();
                    var oldBundleMap = new Dictionary<string, BundleInfo>();
                    foreach (BundleInfo bundleInfo in oldBundleInfos)
                    {
                        oldBundleMap[bundleInfo.Name] = bundleInfo;
                    }

                    var newBundleMap = new Dictionary<string, BundleInfo>();
                    foreach (BundleInfo bundleInfo in newBundleInfos)
                    {
                        newBundleMap[bundleInfo.Name] = bundleInfo;
                    }

                    var needDeleteBundls = new List<BundleInfo>();
                    var needUpdateBundles = new List<BundleInfo>();
                    foreach (var oldBundleInfo in oldBundleInfos)
                    {
                        if (!newBundleMap.ContainsKey(oldBundleInfo.Name))
                        {
                            needDeleteBundls.Add(oldBundleInfo);
                        }
                        else
                        {
                            // just compare hash 
                            if (newBundleMap[oldBundleInfo.Name].hash != oldBundleInfo.hash)
                            {
                                needUpdateBundles.Add(newBundleMap[oldBundleInfo.Name]);
                            }
                        }
                    }

                    foreach (var bundleInfo in needDeleteBundls)
                    {
                        AssetBundleManager.Instance.DeleteBundle(bundleInfo.Name);
                    }

                    _TotalNeedDownSize = 0;
                    _CurrentDownSize = 0;

                    if (needUpdateBundles.Count==0)
                    {
                        needUpdate = false;
                    }
                    foreach (var bundleInfo in needUpdateBundles)
                    {
                        _TotalNeedDownSize += bundleInfo.Size;
                        BeginDownload(bundleInfo.Name, bundleInfo.Size);
                    }
                }

                if (needUpdate)
                {
                    var manifestPath = Path.Combine(AssetBundleManager.Instance.GetDLCPath(),
                        BundleManifest.ManifestBundleName);
                    if (File.Exists(manifestPath))
                    {
                        File.Delete(manifestPath);
                    }
                    File.WriteAllBytes(manifestPath,buff);
                }
                else
                {
                    OnDownComplete?.Invoke();
                }
            }
        }


        private void DownloadFile(string file)
        {
            var webRequest = WebRequest.Create(DownloadURL + "?ver=" + DLCManager.Instance.NewVer + "&file=" + file);
            webRequest.Method = "GET";
            webRequest.ContentType = "application/octet-stream";

            var localPath = Path.Combine(AssetBundleManager.Instance.GetDLCPath(), file);
            var parentPath = Path.GetDirectoryName(localPath);
            if (!Directory.Exists(parentPath))
            {
                Directory.CreateDirectory(parentPath);
            }

            var webResponse = webRequest.GetResponse();
            using (Stream stream = webResponse.GetResponseStream())
            {
                byte[] buffer = new byte[1024];
                if (File.Exists(file))
                {
                    File.Delete(file);
                }

                using (var fileStream = File.OpenWrite(localPath))
                {
                    int len = 0;
                    while ((len = stream.Read(buffer, 0, 1024)) > 0)
                    {
                        fileStream.Write(buffer, 0, len);
                    }
                }
            }
        }


        private void DownloadEntryFile(DownloadEntry entry)
        {
            DownloadFile(entry.name);
            entry.DownloadState = DownloadEntry.State.Success;
        }

        private class DownloadEntry
        {
            public string name;
            public long size;

            public DownloadEntry(string name, long size)
            {
                this.name = name;
                this.size = size;
            }

            public enum State
            {
                Ready,
                Downloading,
                Error,
                Success
            }

            public State DownloadState = State.Ready;


            public Action OnSuccess;
            public Action OnError;
        }

        public void BeginDownload(string name, long size)
        {
            if (!_downloadEntries.ContainsKey(name))
            {
                _downloadEntries.Add(name, new DownloadEntry(name, size));
            }
        }


        private Dictionary<string, DownloadEntry> _downloadEntries = new Dictionary<string, DownloadEntry>();

        private List<string> _needRemovedEntry = new List<string>();

        private void Update()
        {
            foreach (var mapPair in _downloadEntries)
            {
                var entry = mapPair.Value;
                var state = entry.DownloadState;

                if (state == DownloadEntry.State.Ready)
                {
                    entry.DownloadState = DownloadEntry.State.Downloading;
                    DownloadEntryFile(entry);
                    //todo 
                }
                else if (entry.DownloadState == DownloadEntry.State.Error)
                {
                    entry.OnError?.Invoke();
                    _needRemovedEntry.Add(entry.name);
                }
                else if (entry.DownloadState == DownloadEntry.State.Success)
                {
                    _CurrentDownSize += entry.size;
                    OnDownProgress?.Invoke(_TotalNeedDownSize, _CurrentDownSize);
                    entry.OnSuccess?.Invoke();
                    _needRemovedEntry.Add(entry.name);
                }
            }

            if (_needRemovedEntry.Count > 0)
            {
                foreach (var e in _needRemovedEntry)
                {
                    _downloadEntries.Remove(e);
                    if (_downloadEntries.Count == 0)
                    {
                        OnDownComplete?.Invoke();
                        SystemManager.Instance.HideConnectWindow();
                        Debug.Log("download success!");
                    }
                }

                _needRemovedEntry.Clear();
            }
        }
    }
}