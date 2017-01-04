#ifndef _UTIL_H_
#define _UTIL_H_

int strtrimcpy(char* dst, const char* src);
int parse_lines(const char* filename, int buffer_size, int (*callback) (const char* line, int count, void* tag), void* tag);

#endif