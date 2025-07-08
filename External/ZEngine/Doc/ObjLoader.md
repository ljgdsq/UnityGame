public class ObjLoader
{
    public class MeshData
    {
        public List<Vector3> vertices = new List<Vector3>();
        public List<Vector3> normals = new List<Vector3>();
        public List<Vector2> uvs = new List<Vector2>();
        public List<int> triangles = new List<int>();
    }

    public static MeshData LoadObjFile(string filePath)
    {
        MeshData meshData = new MeshData();
        
        // 临时存储原始数据
        List<Vector3> positions = new List<Vector3>();
        List<Vector2> texCoords = new List<Vector2>();
        List<Vector3> normals = new List<Vector3>();
        
        // 存储顶点索引映射，避免重复
        Dictionary<string, int> vertexMap = new Dictionary<string, int>();
        
        // 读取文件
        string[] lines = File.ReadAllLines(filePath);
        
        foreach (string line in lines)
        {
            if (string.IsNullOrEmpty(line) || line.StartsWith("#"))
                continue;
                
            string[] parts = line.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
            if (parts.Length == 0)
                continue;
                
            string type = parts[0].ToLower();
            
            switch (type)
            {
                case "v":  // 顶点位置
                    positions.Add(new Vector3(
                        float.Parse(parts[1], CultureInfo.InvariantCulture),
                        float.Parse(parts[2], CultureInfo.InvariantCulture),
                        float.Parse(parts[3], CultureInfo.InvariantCulture)));
                    break;
                    
                case "vt": // 纹理坐标
                    texCoords.Add(new Vector2(
                        float.Parse(parts[1], CultureInfo.InvariantCulture),
                        float.Parse(parts[2], CultureInfo.InvariantCulture)));
                    break;
                    
                case "vn": // 法线
                    normals.Add(new Vector3(
                        float.Parse(parts[1], CultureInfo.InvariantCulture),
                        float.Parse(parts[2], CultureInfo.InvariantCulture),
                        float.Parse(parts[3], CultureInfo.InvariantCulture)));
                    break;
                    
                case "f":  // 面数据
                    ProcessFace(parts, positions, texCoords, normals, meshData, vertexMap);
                    break;
            }
        }
        
        return meshData;
    }

    private static void ProcessFace(string[] parts, List<Vector3> positions, List<Vector2> texCoords, 
                                   List<Vector3> normals, MeshData meshData, Dictionary<string, int> vertexMap)
    {
        // 处理三角形和四边形面
        int faceVertexCount = parts.Length - 1;
        if (faceVertexCount < 3)
            return;
            
        // 三角化面（如果有超过3个顶点）
        for (int i = 0; i < faceVertexCount - 2; i++)
        {
            AddTriangle(parts[1], parts[i + 2], parts[i + 3], 
                        positions, texCoords, normals, meshData, vertexMap);
        }
    }

    private static void AddTriangle(string v1, string v2, string v3, 
                                  List<Vector3> positions, List<Vector2> texCoords, 
                                  List<Vector3> normals, MeshData meshData, 
                                  Dictionary<string, int> vertexMap)
    {
        int index1 = GetVertexIndex(v1, positions, texCoords, normals, meshData, vertexMap);
        int index2 = GetVertexIndex(v2, positions, texCoords, normals, meshData, vertexMap);
        int index3 = GetVertexIndex(v3, positions, texCoords, normals, meshData, vertexMap);
        
        meshData.triangles.Add(index1);
        meshData.triangles.Add(index2);
        meshData.triangles.Add(index3);
    }

    private static int GetVertexIndex(string vertexData, List<Vector3> positions, 
                                    List<Vector2> texCoords, List<Vector3> normals, 
                                    MeshData meshData, Dictionary<string, int> vertexMap)
    {
        // 检查是否已经处理过这个顶点
        if (vertexMap.TryGetValue(vertexData, out int index))
            return index;
            
        // 解析顶点数据 "vertexIndex/texCoordIndex/normalIndex"
        string[] indices = vertexData.Split('/');
        
        // OBJ索引从1开始，转换为从0开始
        int vertIndex = int.Parse(indices[0]) - 1;
        
        // 添加顶点位置
        meshData.vertices.Add(positions[vertIndex]);
        
        // 添加纹理坐标（如果有）
        if (indices.Length > 1 && indices[1].Length > 0)
        {
            int texIndex = int.Parse(indices[1]) - 1;
            meshData.uvs.Add(texCoords[texIndex]);
        }
        else
        {
            meshData.uvs.Add(Vector2.zero);
        }
        
        // 添加法线（如果有）
        if (indices.Length > 2)
        {
            int normIndex = int.Parse(indices[2]) - 1;
            meshData.normals.Add(normals[normIndex]);
        }
        
        // 记录这个新顶点的索引
        int newIndex = meshData.vertices.Count - 1;
        vertexMap[vertexData] = newIndex;
        
        return newIndex;
    }

    // 创建Unity Mesh的辅助方法
    public static Mesh CreateMesh(MeshData data)
    {
        Mesh mesh = new Mesh();
        mesh.vertices = data.vertices.ToArray();
        mesh.triangles = data.triangles.ToArray();
        
        if (data.uvs.Count == data.vertices.Count)
            mesh.uv = data.uvs.ToArray();
            
        if (data.normals.Count == data.vertices.Count)
            mesh.normals = data.normals.ToArray();
        else
            mesh.RecalculateNormals();
            
        mesh.RecalculateBounds();
        return mesh;
    }
}