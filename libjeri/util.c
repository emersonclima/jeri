#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "util.h"

static const char* lstrip(const char* s) {
	while (*s != '\0' && isspace(*s)) s += 1;
	return s;
}

static const char* rstrip(const char* s) {
	const char* e = s;
	while (*e != '\0') e += 1;	
	while (e != s && (*e == '\0' || isspace(*e))) e -= 1;
	return e;
}

int strtrimcpy(char* dst, const char* src) {
	const char *i = lstrip(src);
	const char *f = rstrip(src);
	int n = i - f;
	n = n > 0 ? n : -n;
	n += 1;
	strncpy(dst, i, n);
	return n;
}

int parse_lines(const char* filename, int buffer_size, int (*callback) (const char* line, int count, void* tag), void* tag) {
	FILE* f = fopen(filename, "r");
	if (!f) return 1;
	char buffer[buffer_size + 1];
	int line = 1;
	while (!feof(f)) {		
		callback(fgets(buffer, buffer_size, f), line, tag);
		line += 1;
	}
	fclose(f);
	return 0;
}