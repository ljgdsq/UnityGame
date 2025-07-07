using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MailSystem : MonoBehaviour
{
    public int UnreadCount { get; private set; }
    public bool HasImportantMail { get; private set; }
    
    public event Action OnMailUpdated;

    public void AddMail(bool important = false)
    {
        Debug.Log($"Adding mail. Important: {important}");
        UnreadCount++;
        if (important) HasImportantMail = true;
        OnMailUpdated?.Invoke();
    }

    public void ReadMail()
    {
        Debug.Log("Reading mail.");
        if (UnreadCount > 0) UnreadCount--;
        if (UnreadCount == 0) HasImportantMail = false;
        OnMailUpdated?.Invoke();
    }
}
