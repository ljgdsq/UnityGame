using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Base;
using SAssetbundle;
using UnityEditor;
using UnityEngine;
using Object = UnityEngine.Object;

public abstract class AssetObject
{
    public readonly string path;
    public Object asset;

    protected AssetObject(string path)
    {
        this.path = path;
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

    public bool isdone;
    public abstract IEnumerator Load();
}


public class BundleAssetObject : AssetObject
{
    public override IEnumerator Load()
    {
        yield return AssetBundleManager.Instance.LoadAssetInBundle(this);
    }

    public BundleAssetObject(string path) : base(path)
    {
    }
}

public class BundleSceneAssetObject : AssetObject
{
    public override IEnumerator Load()
    {
        yield return AssetBundleManager.Instance.LoadSceneInBundle(this);
    }

    public BundleSceneAssetObject(string path) : base(path)
    {
    }
}

#if UNITY_EDITOR
public class EditorAssetObject : AssetObject
{
    public EditorAssetObject(string path) : base(path)
    {
    }

    public override IEnumerator Load()
    {
        asset = AssetDatabase.LoadAssetAtPath<Object>(path);
        yield return null;
        isdone = true;
    }
}
#endif

public class AssetLoadRequest : CustomYieldInstruction
{
    public string path;
    public int index;

    private void CreateAssetObject()
    {
        var inBundle = AssetBundleManager.Instance.IsAssetInBundle(path);
        if (inBundle)
        {
            if (AssetBundleManager.Instance.IsScene(path)) //todo asset do not judge this 
            {
                AssetObject = new BundleSceneAssetObject(path);
            }
            else
            {
                AssetObject = new BundleAssetObject(path);
            }
        }
        else
        {
            isError = true;
        }
    }

    public AssetLoadRequest(string path)
    {
        this.path = path;


#if UNITY_EDITOR
#if Editor_Bundle
        CreateAssetObject();
#else
            AssetObject = new EditorAssetObject(path);
#endif
#else
          CreateAssetObject();
#endif
    }

    public AssetObject AssetObject;

    public bool isError;


    public bool isStart;

    public bool isDone
    {
        get { return AssetObject != null ? AssetObject.isdone : false; }
    }

    public override bool keepWaiting
    {
        get { return !isError && !AssetObject.isdone; }
    }
}

//    public class WindowLoadRequest : AssetLoadRequest
//    {
//        public WindowLoadRequest(string path) : base(path)
//        {
//        }
//
//        public void SetCallback(Action<WindowBase> callback)
//        {
//            callback?.Invoke();
//        }
//        
//    }

public class AssetManager : MonoSingleton<AssetManager>
{
    private List<AssetLoadRequest> _requests = new List<AssetLoadRequest>();

    private Dictionary<string, int> _requestIndex = new Dictionary<string, int>();

    public AssetLoadRequest LoadAsset(string assetPath)
    {
        assetPath = assetPath.ToLower();

        var request = GetRequest(assetPath);
        if (request == null)
        {
            request = CreateRequest(assetPath);
        }

        if (request.AssetObject == null)
        {
            Debug.LogError("can't load asset :" + assetPath);
        }

        return request;
    }

    public AssetLoadRequest LoadScene(string sceneName)
    {
        sceneName = sceneName.ToLower();

        var scenePath = AssetBundleManager.Instance.GetScenePath(sceneName);
        return LoadAsset(scenePath);
    }

    private AssetLoadRequest GetRequest(string assetPath)
    {
        if (_requestIndex.ContainsKey(assetPath))
        {
            return _requests[_requestIndex[assetPath]];
        }

        return null;
    }

    private AssetLoadRequest CreateRequest(string assetPath)
    {
        AssetLoadRequest request = new AssetLoadRequest(assetPath);
        if (request.AssetObject != null)
        {
            _requests.Add(request);
            request.index = _requests.Count - 1;
            _requestIndex.Add(assetPath, request.index);
        }

        return request;
    }




    private void Update()
    {
        if (_requests.Count > 0)
        {
            var request = _requests.First();
            if (request.isDone || request.isError)
            {
                RemoveRequest(request);
            }
            else if (!request.isStart)
            {
                request.isStart = true;
                StartCoroutine(request.AssetObject.Load());
            }
        }
    }

    private void RemoveRequest(AssetLoadRequest request)
    {
        _requests.Remove(request);
        _requestIndex.Remove(request.path);
    }
}