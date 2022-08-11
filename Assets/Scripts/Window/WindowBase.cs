using System;
using DG.Tweening;
using Extension;
using UnityEngine;

namespace Window
{
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = false, Inherited = false)]
    public class WindowPathAttribute:Attribute
    {
        public readonly string ResourcePath;

        public WindowPathAttribute(string resourcePath)
        {
            this.ResourcePath = resourcePath;
        }
    }
    
    //todo use animation asset replace this
    public class WindowBase : MonoBehaviour
    {
        public enum AnimationType
        {
            Fade,
            Scale,
            FadeAndScale,
            Other
        }

        public enum State
        {
            Create,
            Open,
            Opened,
            Close,
            Closed
        }

        public event Action OnCreate;
        public event Action OnOpen;
        public event Action OnOpened;
        public event Action OnClose;
        public event Action OnClosed;

        [SerializeField]
        private Ease InEase = Ease.InOutSine;
        [SerializeField]

        private Ease OutEase= Ease.OutSine;

        [SerializeField] private bool _autoDestroy = true;

        private State _state;

        [SerializeField] private float _animTime = 1.0f;
        [SerializeField] private AnimationType _animationType = AnimationType.Scale;
        private CanvasGroup _canvasGroup;

        private void Awake()
        {
            _state = State.Create;
            _canvasGroup = gameObject.RequireComponent<CanvasGroup>();
            _canvasGroup.alpha = 0;
            gameObject.transform.localScale = Vector3.zero;
            OnCreate?.Invoke();
            Init();
        }


        protected virtual void Init()
        {
            
        }
        
        public void Open()
        {
            if (_state == State.Create)
            {
                _state = State.Open;
                OnOpen?.Invoke();
                PlayOpenAnim();
            }
            else if (_state == State.Closed)
            {
                if (!_autoDestroy)
                {
                    _state = State.Open;
                    OnOpen?.Invoke();
                    PlayOpenAnim();
                }
            }
        }

        public void Close()
        {
            if (_state == State.Opened)
            {
                _state = State.Close;
                OnClose?.Invoke();
                PlayCloseAnim();
            }
        }

        private void ResetScale()
        {
            gameObject.transform.localScale = Vector3.one;
        }

        private void ResetAlpha()
        {
            _canvasGroup.alpha = 1.0f;
        }

        private void PlayOpenAnim()
        {
            void OnPlayEnd()
            {
                _state = State.Opened;
                OnOpened?.Invoke();
            }

            switch (_animationType)
            {
                case AnimationType.Fade:
                    ResetScale();
                    _canvasGroup.DOFade(1, _animTime).OnComplete(OnPlayEnd);
                    break;
                case AnimationType.Scale:
                    ResetAlpha();
                    transform.DOScale(1, _animTime).OnComplete(OnPlayEnd).SetEase(InEase);
                    break;
                case AnimationType.FadeAndScale:
                    transform.DOScale(1, _animTime).SetEase(InEase);
                    _canvasGroup.DOFade(1, _animTime).OnComplete(OnPlayEnd);
                    break;
                case AnimationType.Other:
                    ResetScale();
                    ResetAlpha();
                    PlayCustomOpenAnim();
                    DotweenEx.DoTime(_animTime).OnComplete(OnPlayEnd);
                    break;
            }
        }


        private void PlayCloseAnim()
        {
            void OnPlayEnd()
            {
                _state = State.Closed;
                OnClosed?.Invoke();
                if (_autoDestroy)
                {
                    Destroy(gameObject);
                }
            }

            switch (_animationType)
            {
                case AnimationType.Fade:
                    _canvasGroup.DOFade(0, _animTime).OnComplete(OnPlayEnd);
                    break;
                case AnimationType.Scale:
                    transform.DOScale(0, _animTime).OnComplete(OnPlayEnd).SetEase(OutEase);
                    break;
                case AnimationType.FadeAndScale:
                    transform.DOScale(0, _animTime).SetEase(OutEase);
                    _canvasGroup.DOFade(0, _animTime).OnComplete(OnPlayEnd);
                    break;
                case AnimationType.Other:
                    PlayCustomCloseAnim();
                    DotweenEx.DoTime(_animTime).OnComplete(OnPlayEnd);
                    break;
            }
        }


        protected virtual void PlayCustomOpenAnim()
        {
        }

        protected virtual void PlayCustomCloseAnim()
        {
        }
    }
}