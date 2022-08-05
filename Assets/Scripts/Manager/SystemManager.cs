using AssetManager;
using Base;
using DG.Tweening;
using UnityEngine;

namespace Manager
{
    public class SystemManager : MonoSingleton<SystemManager>
    {
        [SerializeField]
        private Transform root;

        private GameObject ConnectWindow;
        private AssetLoadRequest _connectLoadRequest;


        private void LoadConnectWindow()
        {
            if (ConnectWindow==null)
            {
                if (_connectLoadRequest==null)
                {
                    _connectLoadRequest = AssetManager.AssetManager.Instance.LoadAsset("Assets/Prefabs/UI/ConnectWindow.prefab");
                }
            }
        }

        public void ShowConnectWindow()
        {
            
            if (ConnectWindow==null&& _connectLoadRequest.isDone)
            {
                ConnectWindow =Instantiate(_connectLoadRequest.AssetObject.asset as GameObject,root);
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


        private void Awake()
        {
            LoadConnectWindow();
        }

    }
}