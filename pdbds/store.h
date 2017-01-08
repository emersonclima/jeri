#include "sqlite3.h"
#include "pdb.h"

struct _store;
typedef struct _store store;

int store_open(const char *filename, store **s);
int store_close(store *s);

void store_begin_transaction(store *s);
void store_commit(store* s);
void store_rollback(store* s);

int store_insert_file(store* s, const char* filename);
int store_insert_model(store* s, int file_id);
int store_insert_atom(store* s, int file_id, int model_id, int type, PDBAtom *atom);
