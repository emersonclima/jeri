#include "store.h"
#include "stdbool.h"

struct _dataset;

typedef struct _dataset dataset;

void dataset_open(const char *filename, dataset **ds);
void dataset_close(dataset* ds);
bool dataset_append_file(const char* pdb_filename, dataset* ds);
bool dataset_append_files(char* pdb_filenames[], int count, dataset* ds);