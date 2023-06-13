using System;
using System.IO;
using System.Net;
using System.Threading;
using System.Threading.Tasks;
using UnityEngine;
 
 
public class DownloadHandler 
{
    public struct Result
    {
        public string error;
        public bool isHttpError => !string.IsNullOrEmpty(error);
    }
   
    static int DEFAULT_SLEEP_TIME = 33;
    static int DEFAULT_DOWNLOAD_SPEED = 1024;
    static byte[] DEFAULT_BUFFER = new byte[DEFAULT_DOWNLOAD_SPEED / 30 * 1024];
    static int DEFAULT_DOWNLOAD_TIMEOUT = 5;
 
 
    public event Action<Result> completed;
    public float Progress;
    public ulong DownloadedBytes;
    public bool IsDone;
 
    private string m_File;
    private string m_Url;
    private int m_SleepTime;
    private Result m_Result;
    private Stream m_Stream;
    private FileStream m_FileStream;
    private HttpWebRequest m_Request;
    private HttpWebResponse m_Response;
    private CancellationTokenSource m_Cts;
 
    /// <summary>
    /// 创建下载对象
    /// </summary>
    /// <param name="url">下载路径</param>
    /// <param name="file">保存路径</param>
    /// <param name="speed">每秒最大小速度，KB单位</param>
    public DownloadHandler(string url,string file,int speed)
    {
        m_File = file;
        m_Url = url;
        m_SleepTime = (int)(DEFAULT_SLEEP_TIME * Mathf.Max(1, (float)DEFAULT_DOWNLOAD_SPEED / speed));
    }
 
    //开始下载
    public void StartDownload()
    {
        Download();
    }
    //停止正在下载中的文件
    public void Dispose()
    {
        m_Cts?.Cancel();
        Close();
    }
 
    async void Download()
    {
        m_Cts = new CancellationTokenSource();
        CancellationToken token = m_Cts.Token;
        m_Result = default(Result);
        DownloadedBytes = 0;
        IsDone = false;
        await Task.Run(() =>
        {
            try
            {
                m_Request = (HttpWebRequest)WebRequest.Create(m_Url);
                m_Response = (HttpWebResponse)m_Request.GetResponse();
                long content = m_Response.ContentLength;
                m_Stream = m_Response.GetResponseStream();
                m_Stream.ReadTimeout = DEFAULT_DOWNLOAD_TIMEOUT*1000;
                m_FileStream = new FileStream(m_File, FileMode.Create, FileAccess.Write, FileShare.ReadWrite, DEFAULT_BUFFER.Length);
                int read = 0;
                while (!token.IsCancellationRequested &&
                    (read = m_Stream.Read(DEFAULT_BUFFER, 0, DEFAULT_BUFFER.Length)) > 0)
                {
                    DownloadedBytes += (ulong)read;
                    m_FileStream.Write(DEFAULT_BUFFER, 0, read);
                    Thread.Sleep(m_SleepTime);
                }
            }
            catch (WebException ex)
            {
                m_Result.error = ex.ToString();
            }
            finally
            {
                Close();
            }
 
        }, token);
 
        try
        {
            if (!token.IsCancellationRequested)
            {
                IsDone = true;
                completed?.Invoke(m_Result);
            }
        }
        catch (Exception ex)
        {
            Debug.LogError(ex.ToString());
        }
        
    }
 
    void Close()
    {
        m_FileStream?.Dispose();
        m_Stream?.Dispose();
        m_Response?.Dispose();
        m_Cts?.Dispose();
        m_Cts = null;
        m_FileStream = null;
        m_Stream = null;
        m_Response = null;
        m_Request = null;
    }
 
}