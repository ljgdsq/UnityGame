using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RedDotUIController : MonoBehaviour
{
    [Header("UI Elements")]
    public GameObject redDotSimple;
    public GameObject redDotImportant;
    public Text countText;
    public string redDotKey;


    private void Start() {
        Debug.Log($"Initializing Red Dot UI for key: {redDotKey}");
        var redDotNode = RedDotSystem.Instance.GetNode(redDotKey);
        if (redDotNode!=null) {
            Debug.Log($"Found Red Dot Node: {redDotNode.Key}");
            redDotNode.OnStateChanged += UpdateUI;
            UpdateUI(redDotNode);
        }
    }

    private void UpdateUI(RedDotNode node) {
        Debug.Log($"Updating Red Dot UI for {node.Key} with state {node.State} and count {node.Count}");
        if (redDotSimple) redDotSimple.SetActive(node.State == RedDotState.Normal);
        if (redDotImportant) redDotImportant.SetActive(node.State == RedDotState.Important);
        
        if (countText)
        {
            countText.gameObject.SetActive(node.State == RedDotState.WithCount);
            countText.text = node.Count > 99 ? "99+" : node.Count.ToString();
        }
    }
    private void OnDestroy()
    {
        var node = RedDotSystem.Instance.GetNode(redDotKey);
        if (node != null)
        {
            node.OnStateChanged -= UpdateUI;
        }
    }

}
