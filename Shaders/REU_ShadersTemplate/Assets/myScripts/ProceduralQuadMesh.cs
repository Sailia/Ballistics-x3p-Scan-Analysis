using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProceduralQuadMesh : MonoBehaviour
{

	private Mesh mesh;
	public int xSize, ySize;
	private Vector3[] vertices;


	private void Awake()
	{
		GetComponent<MeshFilter>().mesh = mesh = new Mesh();
		mesh.name = "Procedural Quad";

		vertices = new Vector3[(xSize + 1) * (ySize + 1)];
		Vector2[] uv = new Vector2[vertices.Length];
		for (int i = 0, y = 0; y <= ySize; y++)
		{
			for (int x = 0; x <= xSize; x++, i++)
			{
				vertices[i] = new Vector3(x, 0, y);
				uv[i] = new Vector2((float)x / xSize, (float)y / ySize);
			}
		}

		int[] triangles = new int[xSize * ySize * 6];
		for (int ti = 0, vi = 0, y = 0; y < ySize; y++, vi++)
		{
			for (int x = 0; x < xSize; x++, ti += 6, vi++)
			{
				{
					triangles[ti] = vi;
					triangles[ti + 3] = triangles[ti + 2] = vi + 1;
					triangles[ti + 4] = triangles[ti + 1] = vi + xSize + 1;
					triangles[ti + 5] = vi + xSize + 2;

				}

			}
		}

		mesh.vertices = vertices;
		mesh.uv = uv;
		mesh.triangles = triangles;
		mesh.RecalculateNormals();

	}

	// Start is called before the first frame update
	void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
