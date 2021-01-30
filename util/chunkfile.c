#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "chunkfile.h"

char ChunkFile_Error[128];

const char* ChunkFile_GetError()
{
	return ChunkFile_Error;
}

int ChunkFile_IsParent(Chunk_t* pChunk)
{
	return ((pChunk->size & 0x80000000) != 0);
}

uint32_t ChunkFile_StringToId(const char* pStr)
{
	uint32_t res;
	memcpy(&res, pStr, sizeof(uint32_t));
	return res;
}

int ChunkFile_CreateParentChunk(const uint32_t id, Chunk_t* pChunk)
{
	memset(pChunk, 0, sizeof(Chunk_t));
	
	pChunk->id = id;
	pChunk->size = 0x80000000;
	pChunk->pData = malloc(sizeof(Array_t));
	
	if (pChunk->pData == NULL)
	{
		sprintf(ChunkFile_Error, "Not enough memory");
		return CHUNKFILE_ERROR;
	}
	
	if (Array_Create(sizeof(Chunk_t), 0, (Array_t*) pChunk->pData) == ARRAY_ERROR)
	{
		strcpy(ChunkFile_Error, Array_GetError());
		return CHUNKFILE_ERROR;
	}
	
	return CHUNKFILE_OK;
}

int ChunkFile_CreateDataChunk(const uint32_t id, const uint32_t size, Chunk_t* pChunk)
{
	memset(pChunk, 0, sizeof(Chunk_t));
	
	if (size >= 0x80000000)
	{
		sprintf(ChunkFile_Error, "Chunk to big. Maximum chunksize = ‭2147483647 bytes");
		return CHUNKFILE_ERROR;
	}
	
	pChunk->id = id;
	pChunk->size = size;
	pChunk->pData = malloc(size);
	if (pChunk->pData == NULL)
	{
		sprintf(ChunkFile_Error, "Not enough memory");
		return CHUNKFILE_ERROR;
	}
	
	return CHUNKFILE_OK;
}

void ChunkFile_DestroyChunk(Chunk_t* pChunk)
{
	uint32_t i;
	
	if (pChunk->pData != NULL)
	{
		if ((pChunk->size & 0x80000000) != 0)
		{
			Array_t* pArray = (Array_t*) pChunk->pData;
			for (i=0;i<pArray->numItems;i++)
				ChunkFile_DestroyChunk((Chunk_t*) Array_GetPtr(pArray, i));
			
			Array_Destroy(pArray);
		}
		
		free(pChunk->pData);
		pChunk->pData = NULL;
	}
}

uint32_t ChunkFile_NumOfChilds(Chunk_t* pParent)
{
	if ((pParent->size & 0x80000000) != 0)
		return ((Array_t*) pParent->pData)->numItems;
	
	return 0;
}

Chunk_t* ChunkFile_GetChild(Chunk_t* pParent, const uint32_t index)
{
	if ((pParent->size & 0x80000000) != 0)
		return (Chunk_t*) Array_GetPtr((Array_t*) pParent->pData, index);
	
	return NULL;
}

void ChunkFile_AddChild(Chunk_t* pParent, Chunk_t* pChunk)
{
	if ((pParent->size & 0x80000000) != 0)
		Array_Add((Array_t*) pParent->pData, pChunk);
}

void ChunkFile_RemoveChild(Chunk_t* pParent, const uint32_t index)
{
	if ((pParent->size & 0x80000000) != 0)
		Array_Remove((Array_t*) pParent->pData, index);
}

void ChunkFile_SetData(Chunk_t* pChunk, const void* pData, const uint32_t size)
{
	uint32_t sz = size < pChunk->size ? size : pChunk->size;
	
	memcpy(pChunk->pData, pData, sz);
}

void ChunkFile_GetData(Chunk_t* pChunk, void* pData, const uint32_t size)
{
	uint32_t sz = size < pChunk->size ? size : pChunk->size;
	
	memcpy(pData, pChunk->pData, sz);
}

static int LoadChunk(FILE* pFile, Chunk_t* pChunk)
{
	Chunk_t chunk;
	uint32_t i, numSubChunks, tmp[2];
	
	fread(tmp, 1, 2 * sizeof(uint32_t), pFile);
	if (feof(pFile))
	{
		sprintf(ChunkFile_Error, "Unexpected end of file");
		return CHUNKFILE_ERROR;
	}
	
	if ((tmp[1] & 0x80000000) != 0)
	{
		numSubChunks = tmp[1] & 0x7FFFFFFF;
		
		if (ChunkFile_CreateParentChunk(tmp[0], pChunk) == CHUNKFILE_ERROR)
			return CHUNKFILE_ERROR;
		
		for (i=0;i<numSubChunks;i++)
		{
			if (LoadChunk(pFile, &chunk) == CHUNKFILE_ERROR)
				return CHUNKFILE_ERROR;
			
			ChunkFile_AddChild(pChunk, &chunk);
		}
	}
	else
	{
		if (ChunkFile_CreateDataChunk(tmp[0], tmp[1], pChunk) == CHUNKFILE_ERROR)
			return CHUNKFILE_ERROR;
		
		if (fread(pChunk->pData, 1, tmp[1], pFile) != tmp[1])
		{
			sprintf(ChunkFile_Error, "Unexpected end of chunk data");
			return CHUNKFILE_ERROR;
		}
	}
	
	return CHUNKFILE_OK;
}

int ChunkFile_LoadFromFile(const char* pFilename, Chunk_t* pChunk)
{
	int result = CHUNKFILE_OK;
	FILE* pFile = fopen(pFilename, "rb");
	if (pFile == NULL)
	{
		sprintf(ChunkFile_Error, "Opening file [%s] failed", pFilename);
		return CHUNKFILE_ERROR;
	}
	
	result = LoadChunk(pFile, pChunk);
	
	fclose(pFile);
	return result;
}

int ChunkFile_LoadFromMemory(const uint8_t* pMem, const size_t size, Chunk_t* pChunk)
{
	Chunk_t chunk;
	uint32_t i, numSubChunks;
	
	if (size <= (2 * sizeof(uint32_t)))
		return CHUNKFILE_ERROR;
	
	const uint32_t* pHead = (const uint32_t*) pMem;
	if ((pHead[1] & 0x80000000) != 0)
	{
		numSubChunks = pHead[1] & 0x7FFFFFFF;
		
		if (ChunkFile_CreateParentChunk(pHead[0], pChunk) == CHUNKFILE_ERROR)
			return CHUNKFILE_ERROR;
		
		for (i=0;i<numSubChunks;i++)
		{
			if (ChunkFile_LoadFromMemory((const uint8_t*) &pHead[2], size - 2 * sizeof(uint32_t), &chunk) == CHUNKFILE_ERROR)
				return CHUNKFILE_ERROR;
			
			ChunkFile_AddChild(pChunk, &chunk);
		}
	}
	else
	{
		if (pHead[1] > size)
			return CHUNKFILE_ERROR;
		
		if (ChunkFile_CreateDataChunk(pHead[0], pHead[1], pChunk) == CHUNKFILE_ERROR)
			return CHUNKFILE_ERROR;
		
		memcpy(pChunk->pData, &pHead[2], pHead[1]);
	}
	
	return CHUNKFILE_OK;
}

static int SaveChunk(FILE* pFile, const Chunk_t* pChunk)
{
	uint32_t i, tmp[2];
	
	tmp[0] = pChunk->id;
	if ((pChunk->size & 0x80000000) != 0)
	{
		Array_t* pArray = (Array_t*) pChunk->pData;
		
		tmp[1] = 0x80000000 | pArray->numItems;
		fwrite(tmp, 1, 2 * sizeof(uint32_t), pFile);
		
		for (i=0;i<pArray->numItems;i++)
			if (SaveChunk(pFile, (Chunk_t*) Array_GetPtr(pArray, i)) == CHUNKFILE_ERROR)
				return CHUNKFILE_ERROR;		
	}
	else
	{
		tmp[1] = pChunk->size;
		fwrite(tmp, 1, 2 * sizeof(uint32_t), pFile);
		fwrite(pChunk->pData, 1, tmp[1], pFile);
	}
	
	return CHUNKFILE_OK;
}

int ChunkFile_SaveToFile(const Chunk_t* pChunk, const char* pFilename)
{
	int result = CHUNKFILE_OK;
	FILE* pFile = fopen(pFilename, "wb");
	if (pFile == NULL)
	{
		sprintf(ChunkFile_Error, "Opening file [%s] failed", pFilename);
		return CHUNKFILE_ERROR;
	}
	
	result = SaveChunk(pFile, pChunk);
	
	fclose(pFile);
	return result;
}
