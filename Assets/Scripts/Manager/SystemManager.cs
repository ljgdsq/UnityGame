using System.Collections;
using Base;
using DG.Tweening;
using UnityEngine;

namespace Manager
{
    public class SystemManager : MonoSingleton<SystemManager>
    {
        private Transform root;
        private GameObject ConnectWindow;
        private AssetLoadRequest _connectLoadRequest;


        public override IEnumerator Init()
        {
            yield return LoadSystemUi();
        }

        private IEnumerator LoadSystemUi()
        {
            var req = AssetManager.Instance.LoadAsset("Assets/Prefabs/SystemUI.prefab");
            yield return req;
            Instantiate(req.AssetObject.asset, Instance.transform);
            root = GetComponentInChildren<Canvas>().transform;
        }

        private IEnumerator LoadConnectWindow()
        {
            if (ConnectWindow == null)
            {
                if (_connectLoadRequest == null)
                {
                    _connectLoadRequest = AssetManager.Instance.LoadAsset("Assets/Prefabs/UI/ConnectWindow.prefab");
                    yield return _connectLoadRequest;
                }
            }

            yield return null;
        }

        public void ShowConnectWindow()
        {
            if (ConnectWindow == null && _connectLoadRequest.isDone)
            {
                ConnectWindow = Instantiate(_connectLoadRequest.AssetObject.asset as GameObject, root);
            }

            if (ConnectWindow)
            {
                ConnectWindow.SetActive(true);
            }
        }

        public void HideConnectWindow()
        {
            if (ConnectWindow)
            {
                ConnectWindow.SetActive(false);
            }
        }


        private IEnumerator Start()
        {
           yield return LoadConnectWindow();
        }
    }
}