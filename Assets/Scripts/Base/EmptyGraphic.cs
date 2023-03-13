using UnityEngine.UI;

namespace Base
{
    public class EmptyGraphic: Graphic
    {
        public EmptyGraphic()
        {
            useLegacyMeshGeneration = false;
        }

        protected override void OnPopulateMesh( VertexHelper vh )
        {
            vh.Clear();
        }
    }
}