using UnityEngine;
using UnityEngine.UI;
using System;
using System.Reflection;
using System.Threading.Tasks;

namespace Goose.UIFramework {
    public abstract class UIBase : MonoBehaviour, IUIView {
        #region Fields and Properties

        protected UIConfigAttribute Config { get; private set; }
        protected Canvas Canvas { get; private set; }
        protected CanvasGroup CanvasGroup { get; private set; }
        protected RectTransform RectTransform { get; private set; }
        protected object ViewData { get; private set; }

        public UIType Type => Config.Type;
        public UISize Size => Config.Size;
        public UILayer Layer => Config.Layer;
        public UIState State { get; protected set; }
        public bool IsVisible => CanvasGroup != null && CanvasGroup.alpha > 0;
        public bool IsPersistent => Config.IsPersistent;
        protected bool IsDataLoaded => ViewData != null;

        private int baseOrder;

        // 动画和加载配置
        protected virtual float ShowAnimationDuration => 0.3f;
        protected virtual float HideAnimationDuration => 0.3f;
        protected virtual float DataLoadTimeout => 10f;

        // 数据加载状态
        private TaskCompletionSource<bool> dataLoadingTask;
        private bool isDataLoading;

        #endregion

        #region Unity Lifecycle

        protected virtual void Awake() {
            InitializeComponents();
            State = UIState.None;
        }

        protected virtual void OnEnable() {
            RegisterEvents();
        }

        protected virtual void OnDisable() {
            UnregisterEvents();
        }

        protected virtual void OnDestroy() {
            State = UIState.Destroyed;
            ClearData();
        }

        #endregion

        #region Initialization

        public void SetConfig(UIConfigAttribute config) {
            Config = config;
        }

        private void InitializeComponents() {
            Config = GetType().GetCustomAttribute<UIConfigAttribute>();
            if (Config == null) {
                throw new Exception($"No UIConfig found for {GetType().Name}");
            }

            Canvas = GetComponent<Canvas>();
            CanvasGroup = GetComponent<CanvasGroup>();
            RectTransform = GetComponent<RectTransform>();

            if (Canvas == null) Canvas = gameObject.AddComponent<Canvas>();
            if (CanvasGroup == null) CanvasGroup = gameObject.AddComponent<CanvasGroup>();

            if (RectTransform == null) {
                Debug.LogError($"No RectTransform found on {GetType().Name}");
            }

            baseOrder = Canvas.sortingOrder;
            InitializeLayout();
        }

        private void InitializeLayout() {
            switch (Size) {
                case UISize.FullScreen:
                    SetFullScreen();
                    break;
            }
        }

        #endregion

        #region Event Handling

        protected virtual void RegisterEvents() {
        }

        protected virtual void UnregisterEvents() {
        }

        #endregion

        #region Public Interface

        public virtual async Task Show() {
            if (State == UIState.Showing || State == UIState.Shown) return;

            State = UIState.Showing;

            try {
                // 确保数据已加载
                if (!IsDataLoaded) {
                    await LoadViewData();
                }

                await OnBeforeShow();

                // 更新UI
                OnDataLoaded();
                SetVisible(true);

                await PlayShowAnimation();
                await OnAfterShow();

                State = UIState.Shown;
            }
            catch (Exception ex) {
                Debug.LogError($"Error showing {GetType().Name}: {ex}");
                State = UIState.Hidden;
                throw;
            }
        }

        public virtual async Task Hide() {
            if (State == UIState.Hiding || State == UIState.Hidden) return;

            State = UIState.Hiding;

            try {
                await OnBeforeHide();
                await PlayHideAnimation();
                await OnAfterHide();

                SetVisible(false);

                // 如果不是持久化视图，清理数据
                if (!IsPersistent) {
                    ClearData();
                }

                State = UIState.Hidden;
            }
            catch (Exception ex) {
                Debug.LogError($"Error hiding {GetType().Name}: {ex}");
                State = UIState.Shown;
                throw;
            }
        }

        public virtual async Task Shutdown() {
            try {
                await Hide();
                await OnShutdown();
                State = UIState.Destroyed;
                Destroy(gameObject);
            }
            catch (Exception ex) {
                Debug.LogError($"Error closing {GetType().Name}: {ex}");
                throw;
            }
        }

        public virtual void SetSortingOrder(int order) {
            if (Canvas != null) {
                Canvas.overrideSorting = true;
                Canvas.sortingOrder = order + baseOrder;
            }
        }

        public virtual void SetVisible(bool visible) {
            if (CanvasGroup != null) {
                CanvasGroup.alpha = visible ? 1 : 0;
                CanvasGroup.interactable = visible;
                CanvasGroup.blocksRaycasts = visible;
            }
        }

        #endregion

        #region Data Management

        protected async Task LoadViewData() {
            if (isDataLoading) {
                if (dataLoadingTask != null) {
                    await dataLoadingTask.Task;
                    return;
                }
            }

            if (IsDataLoaded) return;

            try {
                isDataLoading = true;
                dataLoadingTask = new TaskCompletionSource<bool>();

                var timeoutTask = StartTimeoutCheck();
                ViewData = await OnLoadData();

                if (ViewData == null) {
                    throw new Exception($"Failed to load data for {GetType().Name}");
                }

                dataLoadingTask.SetResult(true);
            }
            catch (Exception ex) {
                Debug.LogError($"Error loading data for {GetType().Name}: {ex}");
                dataLoadingTask?.SetException(ex);
                throw;
            }
            finally {
                isDataLoading = false;
            }
        }

        private async Task StartTimeoutCheck() {
            await Task.Delay(TimeSpan.FromSeconds(DataLoadTimeout));
            if (isDataLoading) {
                dataLoadingTask?.TrySetException(
                    new TimeoutException($"Data loading timeout for {GetType().Name}")
                );
            }
        }

        protected void ClearData() {
            ViewData = null;
            OnDataCleared();
        }

        #endregion

        #region Layout Methods

        protected void SetFullScreen() {
            if (RectTransform != null) {
                RectTransform.anchorMin = Vector2.zero;
                RectTransform.anchorMax = Vector2.one;
                RectTransform.offsetMin = Vector2.zero;
                RectTransform.offsetMax = Vector2.zero;
            }
        }

        protected void SetHalfScreen() {
            if (RectTransform != null) {
                RectTransform.anchorMin = new Vector2(0.25f, 0.25f);
                RectTransform.anchorMax = new Vector2(0.75f, 0.75f);
                RectTransform.offsetMin = Vector2.zero;
                RectTransform.offsetMax = Vector2.zero;
            }
        }

        protected void SetQuarterScreen() {
            if (RectTransform != null) {
                RectTransform.anchorMin = new Vector2(0.375f, 0.375f);
                RectTransform.anchorMax = new Vector2(0.625f, 0.625f);
                RectTransform.offsetMin = Vector2.zero;
                RectTransform.offsetMax = Vector2.zero;
            }
        }

        protected void SetSize(Vector2 size) {
            if (RectTransform != null) {
                RectTransform.sizeDelta = size;
            }
        }

        #endregion

        #region Animation

        protected virtual async Task PlayShowAnimation() {
            if (CanvasGroup != null) {
                CanvasGroup.alpha = 0;
                float elapsedTime = 0;

                while (elapsedTime < ShowAnimationDuration) {
                    elapsedTime += Time.deltaTime;
                    float normalizedTime = elapsedTime / ShowAnimationDuration;
                    CanvasGroup.alpha = Mathf.Lerp(0, 1, normalizedTime);
                    await Task.Yield();
                }

                CanvasGroup.alpha = 1;
            }
        }

        protected virtual async Task PlayHideAnimation() {
            if (CanvasGroup != null) {
                CanvasGroup.alpha = 1;
                float elapsedTime = 0;

                while (elapsedTime < HideAnimationDuration) {
                    elapsedTime += Time.deltaTime;
                    float normalizedTime = elapsedTime / HideAnimationDuration;
                    CanvasGroup.alpha = Mathf.Lerp(1, 0, normalizedTime);
                    await Task.Yield();
                }

                CanvasGroup.alpha = 0;
            }
        }

        #endregion

        #region Protected Virtual Methods

        protected virtual Task<object> OnLoadData() => Task.FromResult<object>(new object());

        protected virtual void OnDataLoaded() {
        }

        protected virtual void OnDataCleared() {
        }

        protected virtual Task OnBeforeShow() => Task.CompletedTask;
        protected virtual Task OnAfterShow() => Task.CompletedTask;
        protected virtual Task OnBeforeHide() => Task.CompletedTask;
        protected virtual Task OnAfterHide() => Task.CompletedTask;
        protected virtual Task OnShutdown() => Task.CompletedTask;

        #endregion
    }
}