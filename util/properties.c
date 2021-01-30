#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "properties.h"

static int StringCompare(const char* pStr0, const char* pStr1)
{
	uint32_t i;

	for (i=0;(pStr0[i] != '\0') && (pStr1[i] != '\0');i++)
		if (pStr0[i] != pStr1[i])
			return 0;

	return (pStr0[i] == pStr1[i]);
}

static int PropertiesKeyValuePair_Create(const char* pKey, const char* pValue, PropertiesKeyValuePair_t* pPair)
{
	memset(pPair, 0, sizeof(PropertiesKeyValuePair_t));

	pPair->pKey = (char*) malloc(strlen(pKey) + 1);
	pPair->pValue = (char*) malloc(strlen(pValue) + 1);

	if ((pPair->pKey != NULL) && (pPair->pValue != NULL))
	{
		strcpy(pPair->pKey, pKey);
		strcpy(pPair->pValue, pValue);
		return PROPERTIES_OK;
	}
	
	return PROPERTIES_ERROR;
}

static void PropertiesKeyValuePair_Destroy(PropertiesKeyValuePair_t* pPair)
{
	free(pPair->pValue);
	free(pPair->pKey);
}

int Properties_Create(Properties_t* pProperties)
{
	memset(pProperties, 0, sizeof(Properties_t));
	return (Array_Create(sizeof(PropertiesKeyValuePair_t), PROPERTIES_MAX_INIT, &pProperties->pairs) == ARRAY_ERROR ? PROPERTIES_ERROR : PROPERTIES_OK);
}

int Properties_CreateFromFile(const char* pFilename, Properties_t* pProperties)
{
	uint32_t i;
	char* pLine;
	char* pValue;
	PropertiesKeyValuePair_t pair;
	char line[PROPERTIES_MAX_LINE_LENGTH];

	FILE* pFile = fopen(pFilename, "rt");
	if (pFile == NULL)
		return PROPERTIES_ERROR;
	
	if (Array_Create(sizeof(PropertiesKeyValuePair_t), PROPERTIES_MAX_INIT, &pProperties->pairs) == ARRAY_ERROR)
		return PROPERTIES_ERROR;

	do
	{
		pLine = fgets(line, PROPERTIES_MAX_LINE_LENGTH, pFile);
		
		if (pLine != NULL)
		{
			pValue = NULL;
			for (i=0;(line[i]!='\r')&&(line[i]!='\n')&&(line[i]!='\0');i++)
				if (line[i] != ' ')
					break;
			
			if (line[i] != '#') // Commentline
			{
				pLine = &line[i];
				for (;(line[i]!='\r')&&(line[i]!='\n')&&(line[i]!='\0');i++)
				{
					if (line[i] == '=')
						break;
					else if (line[i] == ' ')
						line[i] = '\0';
				}
					
				if (line[i] == '=')
				{
					line[i++] = '\0';
					for (;(line[i]!='\r')&&(line[i]!='\n')&&(line[i]!='\0');i++)
						if (line[i] != ' ')
							break;

					pValue = &line[i];
					
					for (;(line[i]!='\r')&&(line[i]!='\n')&&(line[i]!='\0');i++);
					line[i] = '\0';
					
					if (PropertiesKeyValuePair_Create(pLine, pValue, &pair) == PROPERTIES_ERROR)
						return PROPERTIES_ERROR;
					
					if (Array_Add(&pProperties->pairs, &pair) == ARRAY_ERROR)
						return PROPERTIES_ERROR;
				}
			}
		}
	}
	while (pLine != NULL);

	fclose(pFile);

	return PROPERTIES_OK;
}

int Properties_SaveToFile(const Properties_t* pProperties, const char* pFilename)
{
	uint32_t i;
	
	FILE* pFile = fopen(pFilename, "wt");
	if (pFile == NULL)
		return PROPERTIES_ERROR;
	
	for (i=0;i<pProperties->pairs.numItems;i++)
	{
		const PropertiesKeyValuePair_t* pPair = (const PropertiesKeyValuePair_t*) Array_GetPtr(&pProperties->pairs, i);
		
		fprintf(pFile, "%s=%s\n", pPair->pKey, pPair->pValue);
	}
	
	fclose(pFile);
	
	return PROPERTIES_OK;
}

void Properties_Destroy(Properties_t* pProperties)
{
	for (uint32_t i=0;i<pProperties->pairs.numItems;i++)
		PropertiesKeyValuePair_Destroy((PropertiesKeyValuePair_t*) Array_GetPtr(&pProperties->pairs, i));

	Array_Destroy(&pProperties->pairs);
}

int Properties_Add(Properties_t* pProperties, const char* pKey, const char* pValue)
{
	PropertiesKeyValuePair_t pair;
	
	if (PropertiesKeyValuePair_Create(pKey, pValue, &pair) == PROPERTIES_ERROR)
		return PROPERTIES_ERROR;
	
	return (Array_Add(&pProperties->pairs, &pair) == ARRAY_ERROR ? PROPERTIES_ERROR : PROPERTIES_OK);
}

void Properties_Remove(Properties_t* pProperties, const char* pKey)
{
	for (uint32_t i=0;i<pProperties->pairs.numItems;i++)
	{
		if (StringCompare(((PropertiesKeyValuePair_t*) Array_GetPtr(&pProperties->pairs, i))->pKey, pKey))
		{
			Array_Remove(&pProperties->pairs, i);
			break;
		}
	}
}

const char* Properties_GetValue(const Properties_t* pProperties, const char* pKey)
{
	for (uint32_t i=0;i<pProperties->pairs.numItems;i++)
	{
		const PropertiesKeyValuePair_t* pPair = (const PropertiesKeyValuePair_t*) Array_GetPtr(&pProperties->pairs, i);
		
		if (StringCompare(pPair->pKey, pKey))
			return pPair->pValue;
	}

	return NULL;
}
