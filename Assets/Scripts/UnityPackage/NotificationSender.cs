#if  UNITY_ANDROID
using System;
using System.Collections.Generic;
using Unity.Notifications.Android;
using UnityEngine;
public class AndroidNotificationSender:MonoBehaviour
{
    private static bool _isInitialized;
    private static List<NotificationInfo> _notificationInfos;
    private static int _notificationId = 1;
    private static void Init()
    {
        if(_isInitialized)
            return;
        _notificationInfos = new List<NotificationInfo>();
        ResetNotificationChannel();
        var notificationGo= new GameObject("NotificationBehaviour").AddComponent<NotificationSender>();
        DontDestroyOnLoad(notificationGo);
        
        _isInitialized = true;
    }

    private static void ResetNotificationChannel()
    {
        _notificationId = 1;
        AndroidNotificationCenter.CancelAllNotifications();//清除上次注册的通知
        var channel = new AndroidNotificationChannel()
        {
            Id = "channel_id",
            Name = "Default Channel",
            Importance = Importance.High,
            Description = "Generic notifications",
            CanShowBadge = true,
            EnableLights=true,
            LockScreenVisibility=LockScreenVisibility.Public
        };
        
        AndroidNotificationCenter.RegisterNotificationChannel(channel);

    }

    protected static void ReSendNotification()
    {
        if (_isInitialized&&_notificationInfos!=null && _notificationInfos.Count > 0)
        {
            ResetNotificationChannel();
            for (var i = 0; i < _notificationInfos.Count; i++)
            {
                SendNotification(_notificationInfos[i]);
            }
        }
     
    }
    
    public static void SendNotification(string title, string text,int day,int hour,int minute,int second ,string smallIconId=null,string largeIconId=null)
    {
        Init();

        var notificationInfo = new NotificationInfo()
        {
            title = title,
            text=text,
            day=day,
            hour = hour,
            minute = minute,
            second = second,
            smallIcon = smallIconId,
            largeIcon = largeIconId
        };
        
        _notificationInfos.Add(notificationInfo);
        SendNotification(notificationInfo);


    }
    
    private static void SendNotification(NotificationInfo notificationInfo)//string title, string text,DateTime time,string smallIconId=null,string largeIconId=null)
    {
        var time = NotificationSender.GetNotificationTime(notificationInfo);
        var notification = new AndroidNotification(){
            Title = notificationInfo.title,
            Text = notificationInfo.text,
            FireTime = time,
            SmallIcon = notificationInfo.smallIcon,
            LargeIcon = notificationInfo.largeIcon,
            Number = _notificationId
        };
        _notificationId++;
        AndroidNotificationCenter.SendNotification(notification, "channel_id");
    }

 
}
#endif

#if  UNITY_IOS
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Notifications.iOS;

public class iOSNotificationSender:MonoBehaviour
{
     private static bool _isInitialized = false;
     private static int _notificationId = 1;
     private static List<NotificationInfo> _notificationInfos;
    // Start is called before the first frame update
    private static void Init()
    {
        if(_isInitialized)
            return;
        _notificationInfos = new List<NotificationInfo>();
        ResetNotificationChannel();
        var notificationGo= new GameObject("NotificationBehaviour").AddComponent<NotificationSender>();
        DontDestroyOnLoad(notificationGo);
        _isInitialized = true;
    }
    private static void ResetNotificationChannel()
    {
        _notificationId = 1;
        iOSNotificationCenter.ApplicationBadge=0;
        iOSNotificationCenter.RemoveAllDeliveredNotifications();
        iOSNotificationCenter.RemoveAllScheduledNotifications();
    }

    protected static void ReSendNotification()
    {
        if (_isInitialized&&_notificationInfos!=null && _notificationInfos.Count > 0)
        {
            ResetNotificationChannel();
            for (var i = 0; i < _notificationInfos.Count; i++)
            {
                SendNotification(_notificationInfos[i]);
            }
        }
     
    }
    

    public static void SendNotification(string title, string text,int day,int hour,int minute,int second ,string smallIconId=null,string largeIconId=null)
    {
        Init();
       
        
        var notificationInfo = new NotificationInfo()
        {
            title = title,
            text=text,
            day=day,
            hour = hour,
            minute = minute,
            second = second,
        };
        _notificationInfos.Add(notificationInfo);
        
        SendNotification(notificationInfo);

    }

 
    private static void SendNotification(NotificationInfo notificationInfo)//string title, string text,TimeSpan timeInterval)
    {
        
        var time = NotificationSender.GetNotificationTime(notificationInfo);
        var timeInterval =time.Subtract(DateTime.Now);
        
        var timeTrigger = new iOSNotificationTimeIntervalTrigger()
        {
            TimeInterval = new TimeSpan(timeInterval.Days, timeInterval.Hours, timeInterval.Minutes, timeInterval.Seconds),// timeInterval,
//            TimeInterval = new TimeSpan(0,0,0,5),// timeInterval,
            Repeats = false
        };

        var notification = new iOSNotification()
        {
            // You can optionally specify a custom identifier which can later be 
            // used to cancel the notification, if you don't set one, a unique 
            // string will be generated automatically.
            Identifier = "_notification_"+ _notificationId,
            Title = notificationInfo.title,
            Body = notificationInfo.text,
            Badge = _notificationId,
            ShowInForeground = true,
            ForegroundPresentationOption = (PresentationOption.Alert | PresentationOption.Sound | PresentationOption.Badge),
            CategoryIdentifier = "category_a",
            ThreadIdentifier = "thread1",
            Trigger = timeTrigger,
        };
        _notificationId++;
        iOSNotificationCenter.ScheduleNotification(notification);
    }

   
}
#endif


public class NotificationInfo
{
    public string title;
    public string text;
    public int day;
    public int hour;
    public int minute;
    public int second;
    public string smallIcon;
    public string largeIcon;
}

public class NotificationSender:
#if UNITY_ANDROID
    AndroidNotificationSender
#else 
    iOSNotificationSender
#endif

{
    private void OnApplicationFocus(bool hasFocus)
    {
        if(hasFocus)
            ReSendNotification();
    }
    
    
    
    /// <summary>
    /// 得到注册通知的时间
    /// </summary>
    /// <returns></returns>
    public static DateTime GetNotificationTime(NotificationInfo notificationInfo)
    {
        var daySpan = new TimeSpan(notificationInfo.day,0,0,0);
        var dateTime=new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, notificationInfo.hour, notificationInfo.minute, notificationInfo.second) ;
        dateTime += daySpan;
        Debug.Log("RegisterNotification:"+dateTime);
        return dateTime;
        
    }
}