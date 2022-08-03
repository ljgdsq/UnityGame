using System;
using DG.Tweening;
using Extension;
using UnityEngine;

public class FadeAnimation : MonoBehaviour
{
    public enum FadeType
    {
        In,
        Out,
        InOut
    }

    [SerializeField] private FadeType _fadeType;
    [SerializeField] private float _fadeTime = 1.0f;
    private CanvasGroup _canvasGroup;

    public Action OnComplete;

    private void Awake()
    {
        _canvasGroup = gameObject.RequireComponent<CanvasGroup>();
        switch (_fadeType)
        {
            case FadeType.In:
                _canvasGroup.alpha = 0;
                _canvasGroup.DOFade(1, _fadeTime).OnComplete(() => { OnComplete?.Invoke(); });
                break;
            case FadeType.Out:
                _canvasGroup.alpha = 1;
                _canvasGroup.DOFade(0, _fadeTime).OnComplete(() => { OnComplete?.Invoke(); });

                break;
            case FadeType.InOut:
                _canvasGroup.alpha = 0;
                _canvasGroup.DOFade(1, _fadeTime / 2).OnComplete(() =>
                {
                    _canvasGroup.DOFade(0, _fadeTime / 2).OnComplete(() => { OnComplete?.Invoke(); });
                });

                break;
        }
    }
}