using SAssetbundle;
using UnityEngine;

public class AssetLoadRequest : CustomYieldInstruction
{
    public string path;
    public int index;

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


    private void CreateAssetObject()
    {
        var inBundle = AssetBundleManager.Instance.IsAssetInBundle(path);
        if (inBundle)
        {
            if (AssetBundleManager.Instance.IsScene(path))  
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
}