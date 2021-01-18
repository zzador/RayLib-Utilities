#ifndef RAYLIBUTIL_H
#define RAYLIBUTIL_H

#include <stdint.h>
#include <raylib.h>
#include <rlgl.h>

extern Mesh CreateMesh(const uint32_t numVertices, const uint32_t numTriangles, const int withNormals, const int withTexcoords, const int indexed);
extern void DestroyMesh(Mesh* pMesh);

extern Material LoadShaderTextureMaterial(const Shader shader, const Texture2D texture);

inline void SetModelParameter(Material* material, const char* modelParamName)
{
	material->shader.locs[LOC_MATRIX_MODEL] = GetShaderLocation(material->shader, modelParamName);
}

inline void SetViewParameter(Material* material, const char* viewParamName)
{
	material->shader.locs[LOC_MATRIX_VIEW] = GetShaderLocation(material->shader, viewParamName);
}

inline void SetProjectionParameter(Material* material, const char* projectionParamName)
{
	material->shader.locs[LOC_MATRIX_PROJECTION] = GetShaderLocation(material->shader, projectionParamName);
}

inline void SetModelViewProjectionParameter(Material* material, const char* mvpParamName)
{
	material->shader.locs[LOC_MATRIX_MVP] = GetShaderLocation(material->shader, mvpParamName);
}

#endif
