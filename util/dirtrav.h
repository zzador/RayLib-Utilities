#ifndef DIRTRAV_H
#define DIRTRAV_H

#define DIRECTORYTRAVERSER_OK		1
#define DIRECTORYTRAVERSER_ERROR	0

#define DIRECTORYTRAVERSER_STOP		0
#define DIRECTORYTRAVERSER_CONTINUE 1

typedef int (*FileHandler)(const char* pPath, const char* pFilename, void* pContext);

extern int DirectoryTraverser_Traverse(const char* pBasePath, FileHandler handler, void* pContext);

#endif
