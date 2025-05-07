using UnityEngine;
using System;
using System.Collections.Generic;

[Serializable]
public class ObjectReference
{
    public string key;
    public UnityEngine.Object reference;
}

public class ObjectReferenceStorage : MonoBehaviour
{
    [SerializeField] private List<ObjectReference> references = new List<ObjectReference>();
    private Dictionary<string, UnityEngine.Object> referenceLookup;

    private void Awake()
    {
        // 建立查找字典
        referenceLookup = new Dictionary<string, UnityEngine.Object>();
        foreach (var reference in references)
        {
            if (!string.IsNullOrEmpty(reference.key) && reference.reference != null)
            {
                referenceLookup[reference.key] = reference.reference;
            }
        }
    }

    // 获取特定类型的引用
    public T Get<T>(string key) where T : UnityEngine.Object
    {
        if (referenceLookup.TryGetValue(key, out UnityEngine.Object value))
        {
            return value as T;
        }
        return null;
    }

    // 在编辑器中添加引用
    public void Add(string key, UnityEngine.Object reference)
    {
        var existingIndex = references.FindIndex(r => r.key == key);
        if (existingIndex >= 0)
        {
            references[existingIndex].reference = reference;
        }
        else
        {
            references.Add(new ObjectReference { key = key, reference = reference });
        }
    }
}
