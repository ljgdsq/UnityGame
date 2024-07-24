using System;
using SAssetbundle;

namespace NewFramework.Service
{
    public class AtlasServices
    {
        protected  void OnInit() {
            UnityEngine.U2D.SpriteAtlasManager.atlasRequested += OnAtlasRequested;
        }

        protected  void OnDestroy() {
            UnityEngine.U2D.SpriteAtlasManager.atlasRequested -= OnAtlasRequested;
        }
        
        private void OnAtlasRequested(string tag, Action<UnityEngine.U2D.SpriteAtlas> action) {
            // var atlas=AssetBundleManager.Instance.LoadAssetInBundle()
            // action.Invoke(atlasRequested);
        }
    }
}