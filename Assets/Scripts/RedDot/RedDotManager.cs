using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RedDotManager : MonoBehaviour
{
    public MailSystem mailSystem;
    
    private void Awake()
    {
        Debug.Log("RedDotManager Start");
        // 初始化红点系统节点
        RedDotSystem.Instance.RegisterNode("Main");
        RedDotSystem.Instance.RegisterNode("Mail", "Main");
        RedDotSystem.Instance.RegisterNode("MailBox", "Mail");
        RedDotSystem.Instance.RegisterNode("MailSystem", "Mail");

        mailSystem.OnMailUpdated += UpdateMailRedDots;
        
        // 设置计算器
        SetupCalculators();
    }
    
    private void SetupCalculators()
    {
        var mailNode = RedDotSystem.Instance.GetNode("Mail");
        var mailBoxNode = RedDotSystem.Instance.GetNode("MailBox");
        var mailSystemNode = RedDotSystem.Instance.GetNode("MailSystem");
        
        mailSystemNode.SetCalculators(
            () => mailSystem.UnreadCount > 0 ? 
                (mailSystem.HasImportantMail ? RedDotState.Important : RedDotState.Normal) 
                : RedDotState.None,
            () => mailSystem.UnreadCount
        );
        
        mailBoxNode.SetCalculators(
            () => mailSystem.UnreadCount > 0 ? RedDotState.Normal : RedDotState.None,
            () => mailSystem.UnreadCount
        );
    }
    
    private void UpdateMailRedDots()
    {
        Debug.Log("Updating Mail Red Dots");
        RedDotSystem.Instance.UpdateNode("MailSystem");
        RedDotSystem.Instance.UpdateNode("MailBox");
    }
    
    // 测试方法
    public void AddNormalMail() => mailSystem.AddMail();
    public void AddImportantMail() => mailSystem.AddMail(true);
    public void ReadMail() => mailSystem.ReadMail();
}
