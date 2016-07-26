#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeri.h"

int print_line(const char* line, int index) {
	if (strstr(line, "ATOM  ") != line) return 0;

	PDBAtom atom;
	PDBAtom_parse(&atom, line);
	PDBAtom_print(&atom);
	return 0;
}

int main(int argc, char* argv[]) {	
	parse_lines("3cyn.pdb", 80, print_line);
}