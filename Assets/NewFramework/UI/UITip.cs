namespace NewFramework.UI
{
    public class UITip:UIBase
    {
        public enum UILayer
        {
            BG=0,
            Window=10000,
            Popup=20000,
            Tip=30000
        }

        
        public enum State
        {
            None,
            Opening,
            Opened,
            Closing,
            Closed
        }

        private int _sortingOrder;
        public string Name;
        public UILayer Layer=UILayer.Window;
        
        public void Open(){}
        public void Close(){}

    }
}