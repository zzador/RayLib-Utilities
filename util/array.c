#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

char Array_Error[128];

#define SIZE_INCREMENT(x) (x<2?2:(x+(x>>1)))

const char* Array_GetError()
{
	return Array_Error;
}

int Array_Create(const uint32_t itemSize, const uint32_t maxItems, Array_t* pArray)
{
	memset(pArray, 0, sizeof(Array_t));
	
	pArray->itemSize = itemSize;
	pArray->maxItems = maxItems;
	pArray->pBuffer = calloc(maxItems, itemSize);
	
	if (pArray->pBuffer == NULL)
	{
		sprintf(Array_Error, "Not enough memory");
		
		return ARRAY_ERROR;
	}
	
	return ARRAY_OK;
}

void Array_Destroy(Array_t* pArray)
{
	if (pArray->pBuffer != NULL)
	{
		free(pArray->pBuffer);
		pArray->pBuffer = NULL;
	}
}

int Array_Add(Array_t* pArray, const void* pItem)
{
	if (pArray->numItems == pArray->maxItems)
	{
		uint32_t newCount = SIZE_INCREMENT(pArray->maxItems);
		void* pBuffer = calloc(newCount, pArray->itemSize);
		if (pBuffer == NULL)
		{
			sprintf(Array_Error, "Not enough memory");
			return ARRAY_ERROR;
		}
		
		memcpy(pBuffer, pArray->pBuffer, pArray->numItems * pArray->itemSize);
		free(pArray->pBuffer);
		pArray->pBuffer = pBuffer;
		pArray->maxItems = newCount;
	}
	
	memcpy(&((uint8_t*) pArray->pBuffer)[(pArray->numItems++) * pArray->itemSize], pItem, pArray->itemSize);
	
	return ARRAY_OK;
}

void Array_Remove(Array_t* pArray, const uint32_t index)
{
	uint32_t offset = index * pArray->itemSize;
	if (index < (pArray->numItems - 1))
		memmove(&((uint8_t*) pArray->pBuffer)[offset], &((uint8_t*) pArray->pBuffer)[offset + pArray->itemSize], pArray->numItems - offset - pArray->itemSize);

	pArray->numItems--;
}

void Array_RemoveLast(Array_t* pArray)
{
	Array_Remove(pArray, pArray->numItems - 1);
}

void* Array_GetPtr(const Array_t* pArray, const uint32_t index)
{
	return &((uint8_t*) pArray->pBuffer)[index * pArray->itemSize];
}

void Array_Get(const Array_t* pArray, const uint32_t index, void* pItem)
{
	memcpy(pItem, &((uint8_t*) pArray->pBuffer)[index * pArray->itemSize], pArray->itemSize);
}

void Array_Set(Array_t* pArray, const uint32_t index, const void* pItem)
{
	memcpy(&((uint8_t*) pArray->pBuffer)[index * pArray->itemSize], pItem, pArray->itemSize);
}
