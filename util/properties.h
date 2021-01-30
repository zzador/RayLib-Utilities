#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <stdint.h>

#include "array.h"

#define PROPERTIES_MAX_INIT         8
#define PROPERTIES_MAX_LINE_LENGTH  1024

enum PropertiesResult
{
	PROPERTIES_ERROR = 0,
	PROPERTIES_OK = 1
};

typedef struct Properties_s Properties_t;
typedef struct PropertiesKeyValuePair_s PropertiesKeyValuePair_t;

struct PropertiesKeyValuePair_s
{
	char* pKey;
	char* pValue;
};

struct Properties_s
{
	Array_t pairs;
};

extern int Properties_Create(Properties_t* pProperties);
extern int Properties_CreateFromFile(const char* pFilename, Properties_t* pProperties);
extern int Properties_SaveToFile(const Properties_t* pProperties, const char* pFilename);
extern void Properties_Destroy(Properties_t* pProperties);

extern int Properties_Add(Properties_t* pProperties, const char* pKey, const char* pValue);
extern void Properties_Remove(Properties_t* pProperties, const char* pKey);

extern const char* Properties_GetValue(const Properties_t* pProperties, const char* pKey);

#endif
