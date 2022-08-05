using System;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using Window;

[WindowPathAttribute("Assets/Prefabs/UI/Dialog.prefab")]
public class Dialog : WindowBase
{
    public event Action OnClickYes;
    public event Action OnClickNo;
    public event Action OnClickClose;

    
   [SerializeField] private Button _closeBtn;
   [SerializeField] private Button _yesBtn;
   [SerializeField] private Button _noBtn;
   
   [SerializeField] private TextMeshProUGUI _titleText;
   [SerializeField] private TextMeshProUGUI _contentText;
   [SerializeField] private Image _contentImage;
    
    
    public enum DialogType
    {
        YesBtn,
        YesNoBtn
    }

    protected override void Init()
    {
        _closeBtn.onClick.AddListener(() =>
        {
            OnClickClose?.Invoke();
            Close();
        });
        _yesBtn.onClick.AddListener(() =>
        {
            OnClickYes?.Invoke();
        });
        _noBtn.onClick.AddListener(() =>
        {
            OnClickNo?.Invoke();
        });
    }

    public void SetType()
    {
                
    }
    
    public void SetTitle(string title)
    {
        _titleText.text = title;
    }

    public void SetContentText(string content)
    {
        _contentText.text = content;
    }


    public void HideCloseBtn()
    {
        _closeBtn.gameObject.SetActive(false);
    }

    public void HideContentImage()
    {
        _contentImage.gameObject.SetActive(false);
    }

    public void HideNoBtn()
    {
        _noBtn.gameObject.SetActive(false);
    }

    public void SetContentImage(Texture2D texture2D, bool snapSize = false)
    {
        
    }
}