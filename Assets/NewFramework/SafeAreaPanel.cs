using UnityEngine;
using UnityEngine.UI;

public class SafeAreaHandlerOptimized : MonoBehaviour
{
    private RectTransform _rectTransform;
    private Rect _lastSafeArea;
    private ScreenOrientation _lastOrientation = ScreenOrientation.AutoRotation;

    void Awake()
    {
        _rectTransform = GetComponent<RectTransform>();
        ApplySafeArea();
        
        // 监听屏幕尺寸和方向变化事件
        Screen.onResolutionChanged += OnResolutionChanged;
        Application.onOrientationChanged += OnOrientationChanged;
    }

    void OnDestroy()
    {
        // 记得取消订阅，防止内存泄漏
        Screen.onResolutionChanged -= OnResolutionChanged;
        Application.onOrientationChanged -= OnOrientationChanged;
    }

    void OnResolutionChanged(Vector2 newResolution)
    {
        ApplySafeArea();
    }

    void OnOrientationChanged(ScreenOrientation newOrientation)
    {
        ApplySafeArea();
    }

    void ApplySafeArea()
    {
        Rect safeArea = Screen.safeArea;
        
        // 如果安全区域和方向都没有变化，则不需要更新
        if (safeArea == _lastSafeArea && Screen.orientation == _lastOrientation)
            return;

        _lastSafeArea = safeArea;
        _lastOrientation = Screen.orientation;

        // 计算并应用安全区域
        Vector2 anchorMin = safeArea.position;
        Vector2 anchorMax = safeArea.position + safeArea.size;
        
        anchorMin.x /= Screen.width;
        anchorMin.y /= Screen.height;
        anchorMax.x /= Screen.width;
        anchorMax.y /= Screen.height;
        
        _rectTransform.anchorMin = anchorMin;
        _rectTransform.anchorMax = anchorMax;
        
        _rectTransform.offsetMin = Vector2.zero;
        _rectTransform.offsetMax = Vector2.zero;
    }
}