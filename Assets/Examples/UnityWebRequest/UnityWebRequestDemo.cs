using System;
using System.Collections;
using System.Collections.Generic;
using Extension;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.UI;
using UnityEngine.Video;
using Base;
using Logger = Base.Logger;

public class UnityWebRequestDemo : MonoBehaviour
{
    private string host = "http://127.0.0.1:8080/";

    [SerializeField]
    private Image _image;
    
    // Start is called before the first frame update
    void Start()
    {
//        StartCoroutine(GetPlanText());
//        StartCoroutine(GetTexture());
        PostJson();
        
    }



    void PostJson()
    {

        var json = @"{""name"":""tom""}";
        
        var request=new UnityWebRequest();
        request.url = host + "PostJson";
        request.method = "POST";
        request.uploadHandler=new UploadHandlerRaw(json.GetUTF8Bytes());
        request.downloadHandler=new DownloadHandlerBuffer();
        request.SetRequestHeader("Content-Type","application/json");


        StartCoroutine(WebRequestHandle(request, (r) =>
        {

            var str = r.downloadHandler.text;
            Logger.Log(str);

        }));
    }
    

    IEnumerator GetPlanText()
    {
        var unityWebRequest = UnityWebRequest.Get(host+"GetPlanText");
        yield return unityWebRequest.SendWebRequest();

        if (CheckIsError(unityWebRequest))
        {
            Debug.Log(unityWebRequest.error);
        }
        else
        {
            Debug.Log(unityWebRequest.downloadHandler.text);

            var result=unityWebRequest.downloadHandler.data;

            print("");
        }
        
    }

    IEnumerator GetTexture()
    {
        
        var unityWebRequest = UnityWebRequestTexture.GetTexture(host+"GetTexture");
        yield return unityWebRequest.SendWebRequest();

        if (CheckIsError(unityWebRequest))
        {
            Debug.Log(unityWebRequest.error);
        }
        else
        {
           // var texture2D = ((DownloadHandlerTexture)unityWebRequest.downloadHandler).texture;

            var texture2D = DownloadHandlerTexture.GetContent(unityWebRequest);
            _image.sprite=Sprite.Create(texture2D,new Rect(0,0,texture2D.width,texture2D.height),Vector2.one/2);
        }
    }


    IEnumerator GetAssetBundle()
    {
        //TODO
        var unityWebRequest = UnityWebRequestAssetBundle.GetAssetBundle("");
        yield return unityWebRequest.SendWebRequest();
        if (CheckIsError(unityWebRequest))
        {
            Debug.Log(unityWebRequest.error);
        }
        else
        {
            var assetBundle = DownloadHandlerAssetBundle.GetContent(unityWebRequest);
        }


    }

    IEnumerator WebRequestHandle(UnityWebRequest request, Action<UnityWebRequest> successAction)
    {
        yield return request.SendWebRequest();
        if (CheckIsError(request))
        {
            Debug.Log(request.error);
        }
        else
        {
            successAction?.Invoke(request);
        }
    }

    bool CheckIsError(UnityWebRequest request)
    {
        if (request.isHttpError || request.isNetworkError)
        {
            return true;
        }

        return false;
    }

    private void LowLevelApi()
    {
        UnityWebRequest request=new UnityWebRequest();
        request.url = "";
        request.method = UnityWebRequest.kHttpVerbGET;
        request.useHttpContinue = false;
        request.chunkedTransfer = false;
        
        
    }
    
    
    
}
