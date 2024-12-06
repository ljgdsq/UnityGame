using System.Threading.Tasks;
using UnityEngine;

namespace Goose.UIFramework {
    public interface IUIView {
        UIType Type { get; }
        UISize Size { get; }
        UILayer Layer { get; }
        UIState State { get; }
        bool IsVisible { get; }
        bool IsPersistent { get; }
        GameObject gameObject { get; }

        Task Show();
        Task Hide();
        Task Shutdown();
        void SetSortingOrder(int order);
        void SetVisible(bool visible);
    }
}