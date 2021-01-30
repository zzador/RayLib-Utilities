#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <stdint.h>

enum ArrayResult
{
	ARRAY_ERROR = 0,
	ARRAY_OK = 1
};

typedef struct Array_s Array_t;

struct Array_s
{
	void* pBuffer;
	uint32_t itemSize, numItems, maxItems; 
};

extern const char* Array_GetError();

extern int Array_Create(const uint32_t itemSize, const uint32_t maxItems, Array_t* pArray);
extern void Array_Destroy(Array_t* pArray);

extern int Array_Add(Array_t* pArray, const void* pItem);
extern void Array_Remove(Array_t* pArray, const uint32_t index);
extern void Array_RemoveLast(Array_t* pArray);

extern void* Array_GetPtr(const Array_t* pArray, const uint32_t index);
extern void Array_Get(const Array_t* pArray, const uint32_t index, void* pItem);
extern void Array_Set(Array_t* pArray, const uint32_t index, const void* pItem);

#endif
