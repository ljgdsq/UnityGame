using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
#if UNITY_EDITOR
using UnityEditor;
#endif
using UnityEngine;
using Object = UnityEngine.Object;

namespace SAssetbundle
{

    public class AssetObject
    {
        public readonly string path;
        public Object asset;


        public AssetObject(string path)
        {
            this.path = path;
        }

        public AssetObject(string path, Object asset)
        {
            this.path = path;
            this.asset = asset;
        }

        public string text
        {
            get
            {
                if (asset is TextAsset textAsset)
                {
                   return textAsset.text;
                }
                return String.Empty;
            }
        }
    }

    public class AssetLoadRequest
    {
        public string path;

        public AssetLoadRequest(string path)
        {
            this.path = path;
        }

        public AssetObject AssetObject;

        public bool isDone;
        public bool isError;


    }
    
    
    public abstract class AssetBundleLoader
    {
        protected List<AssetLoadRequest> _requests=new List<AssetLoadRequest>();
        
        public virtual T LoadAsset<T>(string assetPath)  where T:Object
        {
            return default(T);
        }
        
        public virtual void LoadAssetAsync(AssetLoadRequest request)
        {
       
        }

        protected virtual IEnumerator DoLoad(AssetLoadRequest request)
        {
            yield return null;
        }

        public void Update()
        {
            if (_requests.Count>0)
            {
                var task = _requests.First();
                if (!task.isDone)
                {
                     AssetBundleManager.Instance.StartCoroutine(DoLoad(task));
                }
                else
                {
                    _requests.Remove(task);
                }
            }
        }
    }

#if UNITY_EDITOR
    
    public class AssetBundleLoaderEditor:AssetBundleLoader
    {
        public override T LoadAsset<T>(string assetPath)
        {
       
            return (T)(AssetDatabase.LoadAssetAtPath(assetPath, typeof(T)));
        }


        public override void LoadAssetAsync(AssetLoadRequest request)
        {
            var obj=AssetDatabase.LoadAssetAtPath(request.path, typeof(Object));
            request.isDone = true;
            request.isError = obj == null;
            if ( request.isError)
            {
                request.AssetObject = null;
            }
            else
            {
                request.AssetObject=new AssetObject(request.path,obj);
            }
        }
    }
    #endif

    
    
    public class AssetBundleLoaderEditorBundle:AssetBundleLoader
    {
        public override T LoadAsset<T>(string assetPath)
        {
            return AssetBundleManager.Instance.LoadAsset<T>(assetPath);
        }

        public override void LoadAssetAsync(AssetLoadRequest request)
        {
            _requests.Add(request);
        }

        protected virtual IEnumerator  DoLoad(AssetLoadRequest request)
        {
            AssetObject assetObject=new AssetObject(request.path);
            yield return AssetBundleManager.Instance.LoadAssetInBundle(assetObject);
            
        }
    }

}