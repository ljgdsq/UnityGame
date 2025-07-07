using System;
using System.Collections.Generic;
using UnityEngine;

public enum RedDotState {
    None,
    Normal,
    WithCount,
    Important,
}

public class RedDotNode {
    public string Key { get; private set; }
    public RedDotState State { get; private set; }
    public int Count { get; private set; }
    public event Action<RedDotNode> OnStateChanged;
    
    private Dictionary<string, RedDotNode> children=new();
    private RedDotNode parent;
    private Func<RedDotState> stateCalculator;
    private Func<int> countCalculator;
    public RedDotNode(string key,RedDotNode parent = null, Func<RedDotState> stateCalculator = null, Func<int> countCalculator = null)
    {
        Key = key;
        this.parent = parent;
        this.stateCalculator = stateCalculator;
        this.countCalculator = countCalculator;
        State = RedDotState.None;
        Count = 0;
    }
    
    public void AddChild(RedDotNode child)
    {
        if (children.TryAdd(child.Key, child))
        {
            child.parent = this;
        }
    }
    
    public void RemoveChild(string key)
    {
        if (children.Remove(key, out var child))
        {
            child.parent = null;
        }
    }

    public void UpdateState() {
        Debug.Log($"Updating Mail Red Dots {Key}");
        RedDotState oldState = State;
        int oldCount = Count;
        if (children.Count == 0) {
            if (stateCalculator != null) {
                State = stateCalculator.Invoke();
            }
            
            if (countCalculator != null && State == RedDotState.WithCount) {
                Count = countCalculator.Invoke();
            }else {
                Count = 0;
            }
            
        } else {
            State = RedDotState.None;
            Count = 0;
            foreach (var child in children.Values) {
                if (child.State != RedDotState.None) {
                    State = RedDotState.Normal;
                    if (child.State == RedDotState.WithCount) {
                        Count += child.Count;
                    }
                }
            }

            if (Count>0 && State == RedDotState.Normal) {
                State = RedDotState.WithCount;
            }
        }
        
        if (State != oldState || Count != oldCount) {
            OnStateChanged?.Invoke(this);
            parent?.UpdateState();
        }
        
    }
    
    public void SetCalculators(Func<RedDotState> stateCalculator, Func<int> countCalculator)
    {
        this.stateCalculator = stateCalculator;
        this.countCalculator = countCalculator;
        UpdateState();
    }
    
}


public class RedDotSystem
{
    private static RedDotSystem instance;
    public static RedDotSystem Instance => instance ??= new RedDotSystem();
    
    private Dictionary<string, RedDotNode> nodes = new();
    public RedDotNode Root { get; private set; }
    private RedDotSystem()
    {
        Root = new RedDotNode("Root");
        nodes[Root.Key] = Root;
    }

    public void RegisterNode(string key, string parentKey = "Root") {
        if (nodes.ContainsKey(key)) {
            Debug.LogWarning($"RedDotNode with key '{key}' already exists.");
            return;
        }
        if (!nodes.TryGetValue(parentKey, out var parentNode)) {
            Debug.LogWarning($"Parent RedDotNode with key '{parentKey}' does not exist.");
            return;
        }
        
        var newNode = new RedDotNode(key, parentNode);
        parentNode.AddChild(newNode);
        nodes[key] = newNode;
    }

    public void UpdateNode(string key) {
        if (nodes.TryGetValue(key, out var node)) {
            node.UpdateState();
        }
    }

    public RedDotNode GetNode(string key) {
        nodes.TryGetValue(key, out var node);
        return node;
    }
}
