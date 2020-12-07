#ifndef UTIL_H
#define UTIL_H

void	FixPathSlashes(char* pathbuff);
bool	mkdirRecursive(const char* path, bool includeDotPath = false);

char*	varargs(const char* fmt, ...);
int		xstrsplitws(char* str, char** pointer_array);

#ifndef _WIN32

int stricmp(const char* s1, const char* s2);

#endif

#endif // UTIL_H