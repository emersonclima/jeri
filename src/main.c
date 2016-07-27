#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeri.h"

struct holder {
	int model_serial_counter;
	PDBModel *current_model;	
};

int PDBFile_parse(const char* filename) {
	int model_serial_counter = 0;
	PDBModel* current_model = NULL;
	FILE* f = fopen(filename, "r");
	if (!f) return 1;
	char line[81];	
	int line_index = 1;
	while (!feof(f)) {
		fgets(line, 80, f);
		if (strstr(line, "MODEL ") == line) {
			current_model = PDBModel_create();
			current_model->serial = model_serial_counter;
			model_serial_counter += 1;
			printf("-> MODEL\n");
		} else if (strstr(line, "ATOM  ") == line) {
			if (current_model == NULL) {
				current_model = PDBModel_create();
				current_model->serial = model_serial_counter;
				model_serial_counter += 1;
			}
			PDBAtom* atom = PDBAtom_create();
			PDBAtom_parse(atom, line);
			PDBModel_add(h->current_model, atom);
			//printf("-> ATOM\n");
		} else if (strstr(line, "TER   ") == line) {
			//terminou um modelo	
			printf("-> TER\n");
		}		
		line_index += 1;
	}
	fclose(f);
	return 0;
}

int main(int argc, char* argv[]) {	
	PDBFile_parse("1eer.pdb");
}