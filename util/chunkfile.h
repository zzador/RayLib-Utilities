#ifndef CHUNKFILE_H
#define CHUNKFILE_H

#include <stdint.h>

enum ChunkFileResult
{
	CHUNKFILE_ERROR = 0,
	CHUNKFILE_OK = 1
};

typedef struct Chunk_s Chunk_t;

struct Chunk_s
{
	uint32_t id, size;
	void* pData;
};

extern const char* ChunkFile_GetError();

extern int ChunkFile_IsParent(Chunk_t* pChunk);
extern uint32_t ChunkFile_StringToId(const char* pStr);

extern int ChunkFile_CreateParentChunk(const uint32_t id, Chunk_t* pChunk);
extern int ChunkFile_CreateDataChunk(const uint32_t id, const uint32_t size, Chunk_t* pChunk);
extern void ChunkFile_DestroyChunk(Chunk_t* pChunk);

// Parent chunk
extern uint32_t ChunkFile_NumOfChilds(Chunk_t* pParent);
extern Chunk_t* ChunkFile_GetChild(Chunk_t* pParent, const uint32_t index);
extern void ChunkFile_AddChild(Chunk_t* pParent, Chunk_t* pChunk);
extern void ChunkFile_RemoveChild(Chunk_t* pParent, const uint32_t index);

// Data chunk
extern void ChunkFile_SetData(Chunk_t* pChunk, const void* pData, const uint32_t size);
extern void ChunkFile_GetData(Chunk_t* pChunk, void* pData, const uint32_t size);

// Chunk files
extern int ChunkFile_LoadFromFile(const char* pFilename, Chunk_t* pChunk);
extern int ChunkFile_LoadFromMemory(const uint8_t* pMem, const size_t size, Chunk_t* pChunk);
extern int ChunkFile_SaveToFile(const Chunk_t* pChunk, const char* pFilename);

#endif
