using System.Collections;
using Base;
using DG.Tweening;
using UnityEngine;

namespace Manager
{
    public class SystemManager : MonoSingleton<SystemManager>
    {
        [SerializeField]
        private Transform _systemUIRoot;
        private GameObject ConnectWindow;
        private AssetLoadRequest _connectLoadRequest;

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
            if (ConnectWindow == null && _connectLoadRequest!=null)
            {
                if (_connectLoadRequest.isDone)
                {
                    ConnectWindow = Instantiate(_connectLoadRequest.AssetObject.asset as GameObject, _systemUIRoot);
                }
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
            
            Debug.Log("SystemManager Start");
            
           yield return LoadConnectWindow();
        }
        
        
    }
}