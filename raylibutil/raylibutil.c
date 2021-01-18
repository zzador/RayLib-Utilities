#include <stdlib.h>
#include <string.h>

#include "raylibutil.h"

#define DEFAULT_MESH_VERTEX_BUFFERS 7

Mesh CreateMesh(const uint32_t numVertices, const uint32_t numTriangles, const int withNormals, const int withTexcoords, const int indexed)
{
	Mesh mesh;
	memset(&mesh, 0, sizeof(Mesh));
	
	mesh.vertexCount = (int) numVertices;
	mesh.triangleCount = (int) numTriangles;
	
	mesh.vertices = (float*) RL_CALLOC(numVertices, 3 * sizeof(float));
	memset(mesh.vertices, 0, numVertices * 3 * sizeof(float));
	
	if (withNormals)
	{
		mesh.normals = (float*) RL_CALLOC(numVertices, 3 * sizeof(float));
		memset(mesh.normals, 0, numVertices * 3 * sizeof(float));
	}
	
	if (withTexcoords)
	{
		mesh.texcoords = (float*) RL_CALLOC(numVertices, 2 * sizeof(float));
		memset(mesh.texcoords, 0, numVertices * 2 * sizeof(float));
	}
	
	mesh.vboId = (unsigned int*) RL_CALLOC(DEFAULT_MESH_VERTEX_BUFFERS, sizeof(unsigned int));
	
	if (indexed)
	{
		mesh.indices = (unsigned short*) RL_CALLOC(numTriangles, 3 * sizeof(unsigned short));
		memset(mesh.indices, 0, numTriangles * 3 * sizeof(unsigned short));
	}
	
	/*
    mesh.colors = (unsigned char*) RL_MALLOC(0);
	mesh.tangents = (float*) RL_MALLOC(0);
	mesh.texcoords2 = (float*) RL_MALLOC(0);
	mesh.animVertices = (float*) RL_MALLOC(0);
	mesh.animNormals = (float*) RL_MALLOC(0);
	mesh.boneWeights = (float*) RL_MALLOC(0);
	mesh.boneIds = (int*) RL_MALLOC(0);
	*/
	
	return mesh;
}

void DestroyMesh(Mesh* pMesh)
{
	if (pMesh->vertices != NULL)
	{
		UnloadMesh(*pMesh);
		pMesh->vertices = NULL;
	}
}

Material LoadShaderTextureMaterial(const Shader shader, const Texture2D texture)
{
	Material material = LoadMaterialDefault();
	
	material.shader = shader;
	material.maps[MAP_DIFFUSE].texture = texture;
	
	return material;
}
