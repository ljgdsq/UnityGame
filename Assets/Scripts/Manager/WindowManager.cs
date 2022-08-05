using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using AssetManager;
using Base;
using UnityEngine;
using Window;
using Debug = System.Diagnostics.Debug;

namespace Manager
{
    public class WindowManager : MonoSingleton<WindowManager>
    {
        private Dictionary<Type, string> _windowPath = new Dictionary<Type, string>();

        private Transform root;

        public IEnumerator ShowOkDialog(Action<Dialog> clickOk)
        {
            var path = _windowPath[typeof(Dialog)];
            if (!IsWindowRequestOpen(path))
            {
                var req= RequestOpenWindow(path);
                yield return req;
                _requests.Remove(req);
                var window = Instantiate(req.AssetObject.asset as GameObject,root);
                Dialog dialog = window.GetComponent<Dialog>();
                dialog.HideCloseBtn();
                dialog.HideNoBtn();
                dialog.Open();
                dialog.OnClickYes += () =>
                {
                    clickOk?.Invoke(dialog);
                };
            }
        }

        private List<AssetLoadRequest> _requests=new List<AssetLoadRequest>();

        public void ShowYesNoDialog()
        {
        }

        private AssetLoadRequest RequestOpenWindow(string path)
        {
            var assetLoadRequest = AssetManager.AssetManager.Instance.LoadAsset(path);
            _requests.Add(assetLoadRequest);
            return assetLoadRequest;
        }

        private bool IsWindowRequestOpen(string path)
        {
            foreach (var assetLoadRequest in _requests)
            {
                if (assetLoadRequest.path.Equals(path))
                {
                    return true;
                }
            }

            return false;
        }
        
        public override IEnumerator Init()
        {
            _windowPath.Clear();
            var assembly = Assembly.GetExecutingAssembly();
            var windows = assembly.GetExportedTypes().Where(item => item.IsSubclassOf(typeof(WindowBase)));

            foreach (Type windowType in windows)
            {
                if (windowType.IsDefined(typeof(WindowPathAttribute)))
                {
                    var customAttribute =
                        windowType.GetCustomAttribute(typeof(WindowPathAttribute)) as WindowPathAttribute;
                    Debug.Assert(customAttribute != null, nameof(customAttribute) + " != null");
                    _windowPath.Add(windowType, customAttribute.ResourcePath);
                }
            }

            var request = AssetManager.AssetManager.Instance.LoadAsset("Assets/Prefabs/UI/UIRoot.prefab");
            yield return request;
            var uiRoot=Instantiate(request.AssetObject.asset) as GameObject;
            root = uiRoot.transform;
            yield return null;
        }

        private void Update()
        {
//            if (_requests.Count>0)
//            {
//                var assetLoadRequest = _requests[0];
//                if (assetLoadRequest.isError)
//                {
//                    _requests.Remove(assetLoadRequest);
//                }
//                if (assetLoadRequest.isDone)
//                {
//                    _requests.Remove(assetLoadRequest);
//                    var go = assetLoadRequest.AssetObject.asset as GameObject;
//                    Instantiate(go, root);
//                }
//            }
        }
    }
}