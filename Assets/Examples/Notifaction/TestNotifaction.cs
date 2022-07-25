using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestNotifaction : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        NotificationSender.SendNotification("1111","222222",0,0,0,10);
    }
}
