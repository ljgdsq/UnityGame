using System;
using System.Collections;
using SAssetbundle;
using UnityEngine;
using Object = UnityEngine.Object;
#if UNITY_EDITOR
using UnityEditor;
#endif

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
