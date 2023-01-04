using System.Collections.Generic;
using System.Linq;
using Base;
using SAssetbundle;
using UnityEngine;

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