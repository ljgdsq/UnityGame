using System.Collections;
using Base;
using Net;
using Net.Packet;
using SAssetbundle;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace Manager
{
    public class DLCManager : MonoSingleton<DLCManager>
    {
        public string NewVer;
        public void CheckVersion()
        {
            var checkVersion = new CheckVersion();
            WebApiManager.Instance.Get(checkVersion);
            if (checkVersion.HasNew&&checkVersion.ForceUpdate )
            {
                NewVer = checkVersion.NewVer;
                DownloadManager.Instance.DownloadURL = checkVersion.DownloadURL;
                Debug.Log("find new version :"+checkVersion.NewVer);
                StartCoroutine(WindowManager.Instance.ShowOkDialog((dialog) =>
                {
                    dialog.Close();
                    DownloadNewDLC();
                }));

                DownloadManager.Instance.OnDownComplete += () =>
                {
                    
                    AssetBundleManager.Instance.LoadManifest();
                    StartCoroutine(ToMainScene());
                };
            }
            else
            {
                StartCoroutine(ToMainScene());
            }
        }

        
        
        private IEnumerator ToMainScene()
        {
            var req= AssetManager.AssetManager.Instance.LoadScene("main");
            yield return req;
            SceneManager.LoadScene("main");
        }

        private void DownloadNewDLC()
        {
            DownloadManager.Instance.DownloadManifestTemp();
            DownloadManager.Instance.OnDownProgress += (total, current) =>
            {
                
                Debug.Log($"total {total},current {current} ,percent{current*1.0f/total}");
            };
        }

        private void CheckFileChange()
        {
            AssetBundleManager.Instance.LoadManifest();
            
                        
        }
    }
}