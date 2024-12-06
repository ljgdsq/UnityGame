using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Threading.Tasks;
using UnityEngine;

namespace Goose.UIFramework {
    public class UIManager : MonoBehaviour {
        private static UIManager instance;
        public static UIManager Instance => instance;

        #region Fields

        private Stack<IUIView> viewStack = new Stack<IUIView>();
        private Dictionary<Type, IUIView> viewCache = new Dictionary<Type, IUIView>();
        private HashSet<IUIView> persistentViews = new HashSet<IUIView>();

        [SerializeField] private Transform uiRoot;
        [SerializeField] private Camera uiCamera;

        #endregion

        #region Events

        public event Action<IUIView> OnViewPushed;
        public event Action<IUIView> OnViewPopped;
        public event Action<IUIView> OnViewShown;
        public event Action<IUIView> OnViewHidden;

        #endregion

        #region Unity Methods

        private void Awake() {
            instance = this;
            InitializeUIRoot();
        }

        private void OnDestroy() {
            CleanupAllViews();
        }

        #endregion

        #region Initialization

        private void InitializeUIRoot() {
            if (uiRoot == null) {
                var go = new GameObject("UI Root");
                uiRoot = go.transform;
                go.transform.SetParent(transform);
            }

            if (uiCamera == null) {
                uiCamera = Camera.main;
            }
        }

        #endregion

        #region Core Methods - Push/Pop

        public async Task<T> PushView<T>() where T : UIBase {
            try {
                var newView = await GetOrCreateView<T>();
                var currentView = viewStack.Count > 0 ? viewStack.Peek() : null;

                // 1. 处理栈顶的 dialog
                if (currentView?.Type == UIType.Dialog) {
                    await PopView();
                }

                // 2. 处理全屏窗口的特殊情况
                if (newView.Type == UIType.Window && newView.Size == UISize.FullScreen) {
                    await HandleFullScreenWindow(newView);
                }


                // 显示新视图并加入栈
                viewStack.Push(newView);
                OnViewPushed?.Invoke(newView);
                UpdateViewOrder();

                await newView.Show();
                OnViewShown?.Invoke(newView);

                return newView;
            }
            catch (Exception ex) {
                Debug.LogError($"Failed to push view {typeof(T)}: {ex}");
                throw;
            }
        }

        public async Task<IUIView> PopView() {
            if (viewStack.Count == 0) return null;

            try {
                var currentView = viewStack.Pop();
                if (currentView.IsVisible) {
                    await currentView.Hide();
                    OnViewHidden?.Invoke(currentView);
                }

                // 处理非持久化视图
                if (!currentView.IsPersistent) {
                    viewCache.Remove(currentView.GetType());
                    await currentView.Shutdown();
                }

                OnViewPopped?.Invoke(currentView);

                // 如果当前是全屏窗口，显示上一个视图
                var previousView = viewStack.Count > 0 ? viewStack.Peek() : null;
                if (previousView != null && currentView.Size == UISize.FullScreen) {
                    await previousView.Show();
                    OnViewShown?.Invoke(previousView);
                }

                UpdateViewOrder();
                return previousView;
            }
            catch (Exception ex) {
                Debug.LogError($"Failed to pop view: {ex}");
                throw;
            }
        }

        public async Task PopToView<T>() where T : UIBase {
            if (viewStack.Count == 0) return;

            try {
                var targetView = viewStack.FirstOrDefault(v => v is T);
                if (targetView == null) return;

                while (viewStack.Count > 0 && viewStack.Peek() != targetView) {
                    await PopView();
                }
            }
            catch (Exception ex) {
                Debug.LogError($"Failed to pop to view {typeof(T)}: {ex}");
                throw;
            }
        }

        public async Task PopAllViews(bool includePersistent = false) {
            while (viewStack.Count > 0) {
                var view = viewStack.Peek();
                if (!includePersistent && view.IsPersistent) break;
                await PopView();
            }
        }

        #endregion

        #region View Management

        private async Task<T> GetOrCreateView<T>() where T : UIBase {
            var type = typeof(T);

            // 检查缓存
            if (viewCache.TryGetValue(type, out var cachedView)) {
                return (T)cachedView;
            }

            // 加载并创建
            var config = type.GetCustomAttribute<UIConfigAttribute>();
            if (config == null) throw new Exception($"No UIConfig found for {type}");

            var prefab = await LoadViewPrefab(config.ResourcePath);
            var instance = Instantiate(prefab, uiRoot);
            var view = instance.GetComponent<T>();
            // view.SetConfig(config);

            if (view == null) throw new Exception($"No view component found on prefab {config.ResourcePath}");

            // 缓存持久化视图
            if (config.IsPersistent) {
                viewCache[type] = view;
                persistentViews.Add(view);
            }

            return view;
        }

        private async Task HandleCurrentDialog(IUIView currentView) {
            if (currentView.Type == UIType.Dialog) {
                viewStack.Pop();
                await currentView.Shutdown();
                OnViewHidden?.Invoke(currentView);
            }
        }

        private async Task HandleFullScreenWindow(IUIView newView) {
            var tasks = new List<Task>();
            var viewsToProcess = viewStack.ToList();

            foreach (var view in viewsToProcess) {
                if (view == newView) continue;

                if (view.Type == UIType.Dialog && !view.IsPersistent) {
                    tasks.Add(CloseAndRemoveView(view));
                } else {
                    tasks.Add(view.Hide());
                    OnViewHidden?.Invoke(view);
                }
            }

            await Task.WhenAll(tasks);
        }

        private async Task CloseAndRemoveView(IUIView view) {
            await view.Shutdown();
            viewStack = new Stack<IUIView>(viewStack.Where(v => v != view));

            if (!view.IsPersistent) {
                Destroy(view.gameObject);
            }
        }

        private void UpdateViewOrder() {
            int order = 0;
            foreach (var view in viewStack.Reverse()) {
                int baseOrder = (int)view.Layer;
                view.SetSortingOrder(baseOrder + (order * 10));
                order++;
            }
        }

        #endregion

        #region Resource Loading

        private async Task<GameObject> LoadViewPrefab(string path) {
            // 这里替换为你的资源加载系统
            var prefab = Resources.Load<GameObject>(path);
            if (prefab == null) {
                throw new Exception($"Failed to load prefab at path: {path}");
            }

            return prefab;
        }

        #endregion

        #region Utility Methods

        public T GetView<T>() where T : UIBase {
            return viewStack.OfType<T>().FirstOrDefault();
        }

        public bool IsViewInStack<T>() where T : UIBase {
            return viewStack.Any(v => v is T);
        }

        public bool IsTopView<T>() where T : UIBase {
            if (viewStack.Count == 0) return false;
            return viewStack.Peek() is T;
        }

        public IReadOnlyList<IUIView> GetViewStack() {
            return viewStack.ToList().AsReadOnly();
        }

        public void PrintViewStack() {
            Debug.Log("Current View Stack:");
            foreach (var view in viewStack.Reverse()) {
                Debug.Log($"- {view.GetType().Name} (Visible: {view.IsVisible})");
            }
        }

        private void CleanupAllViews() {
            foreach (var view in viewStack) {
                if (view != null && view.gameObject != null) {
                    Destroy(view.gameObject);
                }
            }

            viewStack.Clear();
            viewCache.Clear();
            persistentViews.Clear();
        }

        #endregion
    }
}