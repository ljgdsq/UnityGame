using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Graphic))]
public class MyGraphic : MaskableGraphic
{
    [SerializeField] [Range(3, 100)] private int segmentCount = 3;

    private float length = 100f;

    protected override void OnPopulateMesh(VertexHelper vh)
    {
        base.OnPopulateMesh(vh);

        vh.Clear();

        float radius = length / 2f;
        Vector2 center = new Vector2(rectTransform.rect.xMin + radius, rectTransform.rect.yMin + radius);

        float angle = 0;
        float angleIncrement = 360f / segmentCount;

        UIVertex vert = UIVertex.simpleVert;

        for (int i = 0; i < segmentCount; i++)
        {
            vert.position = center + new Vector2(Mathf.Sin(angle * Mathf.Deg2Rad) * radius,
                                Mathf.Cos(angle * Mathf.Deg2Rad) * radius);
            vert.color = color;
            vert.uv0 = Vector2.zero;

            vh.AddVert(vert);

            angle += angleIncrement;
        }

        int vertexCount = vh.currentVertCount;
        int triangleCount = vertexCount - 2;

        for (int i = 0; i < triangleCount; i++)
        {
            vh.AddTriangle(0, i + 1, i + 2);
        }
    }

}
  