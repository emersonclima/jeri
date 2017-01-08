#include "dataset.h"
#include "store.h"
#include "string.h"
#include "malloc.h"
#include "stdio.h"
#include "stdlib.h"

struct _dataset {
	store* store;
};

void dataset_open(const char *filename, dataset **ds) {	
	dataset* aux = (dataset*) malloc(sizeof(dataset));	
	store_open(filename, &(aux->store));
	*ds = aux;
}

void dataset_close(dataset* ds) {
	store_close(ds->store);
}

static bool _dataset_append_file(const char* pdb_filename, dataset* ds) {
	FILE* in = fopen(pdb_filename, "r");
	if (in == NULL) {
		fprintf(stderr, "Falha ao abrir arquivo %s\n", pdb_filename);
		return false;
	}

	int file_id = store_insert_file(ds->store, pdb_filename);	
	int model_id = 0;	
	char buffer[81];
	PDBAtom atom;
	while (fgets(buffer, 81, in) != NULL) {
		bool is_atom = strstr(buffer, "ATOM   ") == buffer;		
		bool is_hetatm = strstr(buffer, "HETATM ") == buffer;
		bool is_model = strstr(buffer, "MODEL  ") == buffer;

		if (is_model) {
			model_id = store_insert_model(ds->store, file_id);			
		} else if (is_atom || is_hetatm) {
			if (model_id == 0) {
				model_id = store_insert_model(ds->store, file_id);
			}
			PDBAtom_parse(&atom, buffer);
			//PDBAtom_print(&atom);
			store_insert_atom(ds->store, file_id, model_id, is_atom ? 0 : 1, &atom);			
		}
	}
	fclose(in);
}

bool dataset_append_file(const char* pdb_filename, dataset* ds) {
	store_begin_transaction(ds->store);
	_dataset_append_file(pdb_filename, ds);
	store_commit(ds->store);
}

bool dataset_append_files(char* pdb_filenames[], int count, dataset* ds) {
	store_begin_transaction(ds->store);
	for (int i = 0; i < count; i++) {
		_dataset_append_file((const char*) pdb_filenames[i], ds);
	}
	store_commit(ds->store);
}