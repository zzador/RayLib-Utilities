#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "dirtrav.h"

int DirectoryTraverser_Traverse(const char* pBasePath, FileHandler handler, void* pContext)
{
	BOOL tResult;
	HANDLE tHandle;
	WIN32_FIND_DATAA fileInfo;
	memset(&fileInfo, 0, sizeof(WIN32_FIND_DATAA));
	char* pFilename = (char*) malloc(strlen(pBasePath)+3);
	sprintf(pFilename, "%s/*", pBasePath);
	
	tResult = TRUE;
	tHandle = FindFirstFileA(pFilename, &fileInfo);
	while ((tHandle != INVALID_HANDLE_VALUE) && tResult)
	{
		if ((fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			if ((strcmp(fileInfo.cFileName, ".") != 0) && (strcmp(fileInfo.cFileName, "..") != 0))
			{
				char* pNewBase = (char*) malloc(strlen(pBasePath)+strlen(fileInfo.cFileName)+2);
				sprintf(pNewBase, "%s/%s", pBasePath, fileInfo.cFileName);
				
				if (DirectoryTraverser_Traverse(pNewBase, handler, pContext) == DIRECTORYTRAVERSER_ERROR)
				{
					free(pNewBase);
					FindClose(tHandle);
					free(pFilename);
					
					return DIRECTORYTRAVERSER_ERROR;
				}
				
				free(pNewBase);
			}
		}
		else if (handler(pBasePath, fileInfo.cFileName, pContext) == DIRECTORYTRAVERSER_STOP)
		{
			FindClose(tHandle);
			free(pFilename);
			
			return DIRECTORYTRAVERSER_ERROR;
		}
			
		tResult = FindNextFileA(tHandle, &fileInfo);
	}
	
	FindClose(tHandle);
	free(pFilename);
	
	return DIRECTORYTRAVERSER_OK;
}
