using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Graphic))]
public class MyGraphic2 : MaskableGraphic
{
    [System.Serializable]
    public class Polygon
    {
        public float length = 50f;
        public Color color = Color.white;
    }

    [SerializeField]
    private Polygon[] polygons = new Polygon[]
    {
        new Polygon { length = 30f, color = Color.red },
        new Polygon { length = 50f, color = Color.green },
        new Polygon { length = 70f, color = Color.blue }
    };
    [SerializeField] [Range(3, 100)] private int segmentCount = 3;

    private float length = 100f;

    protected override void OnPopulateMesh(VertexHelper vh)
    {
        base.OnPopulateMesh(vh);

        vh.Clear();

        Vector2[] vertices = new Vector2[segmentCount];
        UIVertex[] outerVertices = new UIVertex[segmentCount];

        float outerRadius = length / 2f;

        Vector2 center = rectTransform.pivot;
        center = new Vector2(center.x * rectTransform.rect.width, center.y * rectTransform.rect.height);

        for (int j = 0; j < polygons.Length; j++)
        {
            float innerRadius = polygons[j].length / 2f;

            float angle = 0;
            float angleIncrement = 360f / segmentCount;

            for (int i = 0; i < segmentCount; i++)
            {
                Vector2 outerVertex = center + new Vector2(Mathf.Sin(angle * Mathf.Deg2Rad) * outerRadius, Mathf.Cos(angle * Mathf.Deg2Rad) * outerRadius);
                Vector2 innerVertex = center + new Vector2(Mathf.Sin(angle * Mathf.Deg2Rad) * innerRadius, Mathf.Cos(angle * Mathf.Deg2Rad) * innerRadius);

                outerVertices[i] = new UIVertex();
                outerVertices[i].position = outerVertex;
                outerVertices[i].color = polygons[j].color;
                outerVertices[i].uv0 = Vector2.zero;

                vertices[i] = innerVertex;

                angle += angleIncrement;
            }

            for (int i = 0; i < segmentCount; i++)
            {
                UIVertex vertex1 = outerVertices[i];
                UIVertex vertex2 = new UIVertex();
                vertex2.position = vertices[i];
                vertex2.color = polygons[j].color;
                vertex2.uv0 = Vector2.zero;

                UIVertex vertex3 = new UIVertex();
                vertex3.position = vertices[(i + 1) % segmentCount];
                vertex3.color = polygons[j].color;
                vertex3.uv0 = Vector2.zero;

                UIVertex vertex4 = outerVertices[(i + 1) % segmentCount];

                vh.AddUIVertexQuad(new UIVertex[] { vertex1, vertex2, vertex3, vertex4 });
            }

            outerRadius = innerRadius;
        }
    }

#if UNITY_EDITOR
    protected override void OnValidate()
    {
        base.OnValidate();

        // 强制重绘 MyGraphic 面板区域
        SetVerticesDirty();
    }
#endif
}