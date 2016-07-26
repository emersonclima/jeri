#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "jeri.h"

void test_strtrimcpy() {
	char s[6];	
	int n;

	strcpy(s, "a b");
	n = strtrimcpy(s, s);
	s[n] = '\0';	
	assert(strcmp(s, "a b") == 0);

	strcpy(s, "  a b"); 
	n = strtrimcpy(s, s);
	s[n] = '\0';
	assert(strcmp(s, "a b") == 0);

	strcpy(s, "a b  "); 
	n = strtrimcpy(s, s);
	s[n] = '\0';	
	assert(strcmp(s, "a b") == 0);

	strcpy(s, " a b "); 
	n = strtrimcpy(s, s);
	s[n] = '\0';
	assert(strcmp(s, "a b") == 0);
	
	strcpy(s, "a"); 
	n = strtrimcpy(s, s);
	s[n] = '\0';
	assert(strcmp(s, "a") == 0);
	
	strcpy(s, ""); 
	n = strtrimcpy(s, s);
	s[n] = '\0';
	assert(strcmp(s, "") == 0);
}

int main() {
	test_strtrimcpy();
}