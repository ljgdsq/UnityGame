using System.Collections.Generic;
using UnityEngine;

namespace UIToolkitExamples
{
    [CreateAssetMenu(menuName = "UIToolkitExamples/TexturePackAsset")]
    public class TexturePackAsset : ScriptableObject
    {
        public List<Texture2D> textures;

        public void Reset()
        {
            textures = new List<Texture2D>(){ null, null, null, null };
        }
    }
}