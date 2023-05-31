using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Graphic), typeof(CanvasRenderer))]
public class MyGraphic3 : MaskableGraphic
{
    [SerializeField] private int segmentCount = 5;
    [SerializeField] private float[] edgeLengths = new float[] { 50, 50, 50, 50, 50 };
    [SerializeField] private float lineWidth = 4f;
    [SerializeField] private Color lineColor = Color.white;

    protected override void OnPopulateMesh(VertexHelper vh)
    {
        vh.Clear();

        Vector2[] vertices = new Vector2[segmentCount];
        UIVertex[] lineVertices = new UIVertex[4];

        float radius = rectTransform.rect.width / 2 - lineWidth / 2;
        Vector2 center = rectTransform.pivot * rectTransform.rect.size;

        float angleIncrement = 2 * Mathf.PI / segmentCount;
        float angle = Mathf.PI / 2;

        for (int i = 0; i < segmentCount; i++)
        {
            vertices[i] = new Vector2(Mathf.Cos(angle), Mathf.Sin(angle)) * radius + center;
            angle += angleIncrement;
        }

        for (int i = 0; i < segmentCount; i++)
        {
            int nextIndex = (i + 1) % segmentCount;
            Vector2 direction = (vertices[nextIndex] - vertices[i]).normalized;
            Vector2 perpendicular = new Vector2(-direction.y, direction.x);
            perpendicular *= lineWidth / 2f;

            lineVertices[0].position = vertices[i] + perpendicular; // 左上点
            lineVertices[1].position = vertices[nextIndex] + perpendicular; // 右上点
            lineVertices[2].position = vertices[nextIndex] - perpendicular; // 右下点
            lineVertices[3].position = vertices[i] - perpendicular; // 左下点

            for (int j = 0; j < 4; j++)
            {
                lineVertices[j].color = lineColor;
                lineVertices[j].uv0 = Vector2.zero;
            }

            vh.AddUIVertexQuad(lineVertices);
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