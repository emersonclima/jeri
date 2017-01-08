#include "stdlib.h"
#include "stdio.h"
#include "sqlite3.h"
#include "pdb.h"
#include "store.h"

struct _store {	
	sqlite3* db;
	sqlite3_stmt *begin_transaction_stmt;
	sqlite3_stmt *commit_stmt;
	sqlite3_stmt *rollback_stmt;
	sqlite3_stmt *insert_file_stmt;
	sqlite3_stmt *insert_model_stmt;
	sqlite3_stmt *insert_atom_stmt;
};

static void sqlite3_bind_text_helper(sqlite3 *db, sqlite3_stmt *stmt, int index, const char* text, int count) {
	int rc = sqlite3_bind_text(stmt, index, text, count, SQLITE_TRANSIENT);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "fail (%d) binding sqlite3 text: %s\n", rc, sqlite3_errmsg(db));		
        sqlite3_close(db);
		exit(1);
	}
}

static void sqlite3_bind_int_helper(sqlite3 *db, sqlite3_stmt *stmt, int index, int value) {
	if (sqlite3_bind_int(stmt, index, value) != SQLITE_OK) {
		fprintf(stderr, "fail binding sqlite3 int: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
}

static void sqlite3_bind_double_helper(sqlite3 *db, sqlite3_stmt *stmt, int index, double value) {
	if (sqlite3_bind_double(stmt, index, value) != SQLITE_OK) {
		fprintf(stderr, "fail binding sqlite3 double: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
}

static void sqlite3_exec_helper(sqlite3 *db, char* sql) {
	char* err_msg = NULL;
	if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
		fprintf(stderr, "sqlite3_exec error: %s\n", sqlite3_errmsg(db));
        sqlite3_free(err_msg);        
        sqlite3_close(db);
	};
}

static char* store_schema = "begin transaction;"
	"create table if not exists about (name varchar, description varchar, author varchar, date varchar);"
	"create table if not exists file (id integer primary key, name varchar);"
	"create table if not exists model (id integer primary key, file_id integer);"
	"create table if not exists atom (id primary key, file_id integer, model_id integer, name varchar, altLoc varchar, resName varchar, chainID varchar, resSeq integer, iCode varchar, x real, y real, z real, occupancy real, tempFactor real, element varchar, charge varchar);"
	"commit;";

int store_open(const char *filename, store **s) {
	store* aux = (store*) malloc(sizeof(store));
	sqlite3_open(filename, &(aux->db));
	sqlite3_prepare_v2(aux->db, "begin transaction", -1, &(aux->begin_transaction_stmt), 0);
	sqlite3_prepare_v2(aux->db, "commit", -1, &(aux->commit_stmt), 0);
	sqlite3_prepare_v2(aux->db, "rollback", -1, &(aux->rollback_stmt), 0);
	sqlite3_prepare_v2(aux->db, "insert into file (name) values (?)", -1, &(aux->insert_file_stmt), 0);
	sqlite3_prepare_v2(aux->db, "insert into model (file_id) values (?)", -1, &(aux->insert_model_stmt), 0);
	sqlite3_prepare_v2(aux->db, "insert into atom (file_id, model_id, name, altLoc, resName, chainID, resSeq, iCode, x, y, z, occupancy, tempFactor, element, charge) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", -1, &(aux->insert_atom_stmt), 0);
	sqlite3_exec_helper(aux->db, store_schema);
	*s = aux;
}

int store_close(store* s) {
	sqlite3_close(s->db);
	sqlite3_finalize(s->insert_file_stmt);
	sqlite3_finalize(s->insert_model_stmt);
	sqlite3_finalize(s->insert_atom_stmt);
}

void store_begin_transaction(store* s) {
	if (sqlite3_step(s->begin_transaction_stmt) != SQLITE_DONE) {
		fprintf(stderr, "fail begin transaction: %s\n", sqlite3_errmsg(s->db));
		exit(1);
	}
}

void store_commit(store* s) {
	if (sqlite3_step(s->commit_stmt) != SQLITE_DONE) {
		fprintf(stderr, "fail commit: %s\n", sqlite3_errmsg(s->db));
		exit(1);
	}
}

void store_rollback(store* s) {	
	if (sqlite3_step(s->rollback_stmt) != SQLITE_DONE) {
		fprintf(stderr, "fail rollback: %s\n", sqlite3_errmsg(s->db));
		exit(1);
	}
}

int store_insert_file(store* store, const char* filename) {
	sqlite3_clear_bindings(store->insert_file_stmt);
	sqlite3_bind_text_helper(store->db, store->insert_file_stmt, 1, filename, -1);

	if (sqlite3_step(store->insert_file_stmt) != SQLITE_DONE) {
		fprintf(stderr, "sqlite3: %s\n", sqlite3_errmsg(store->db));
		exit(1);
	}

	sqlite3_clear_bindings(store->insert_file_stmt);
	sqlite3_reset(store->insert_file_stmt);	
	
	return sqlite3_last_insert_rowid(store->db);	
}

int store_insert_model(store* store, int file_id) {
	sqlite3_bind_int_helper(store->db, store->insert_model_stmt, 1, file_id);

	if (sqlite3_step(store->insert_model_stmt) != SQLITE_DONE) {
		fprintf(stderr, "sqlite3: %s\n", sqlite3_errmsg(store->db));
		exit(1);
	}

	sqlite3_clear_bindings(store->insert_model_stmt);
	sqlite3_reset(store->insert_model_stmt);
	
	return sqlite3_last_insert_rowid(store->db);	
}

int store_insert_atom(store* store, int file_id, int model_id, int type, PDBAtom *atom) {
	sqlite3_bind_int_helper(store->db, store->insert_atom_stmt, 1, file_id);
	sqlite3_bind_int_helper(store->db, store->insert_atom_stmt, 2, model_id);
	sqlite3_bind_text_helper(store->db, store->insert_atom_stmt, 3, atom->name, -1);
	
	char aux[2];
	sprintf(aux, "%c", atom->altLoc);
	sqlite3_bind_text_helper(store->db, store->insert_atom_stmt, 4, aux, -1);
	
	sqlite3_bind_text_helper(store->db, store->insert_atom_stmt, 5, atom->resName, -1);
	
	sprintf(aux, "%c", atom->chainID);
	sqlite3_bind_text_helper(store->db, store->insert_atom_stmt, 6, aux, -1);
	
	sqlite3_bind_int_helper(store->db, store->insert_atom_stmt, 7, atom->resSeq);
	
	sprintf(aux, "%c", atom->iCode);
	sqlite3_bind_text_helper(store->db, store->insert_atom_stmt, 8, aux, -1);
	
	sqlite3_bind_double_helper(store->db, store->insert_atom_stmt, 9, atom->x);
	sqlite3_bind_double_helper(store->db, store->insert_atom_stmt, 10, atom->y);
	sqlite3_bind_double_helper(store->db, store->insert_atom_stmt, 11, atom->z);
	sqlite3_bind_double_helper(store->db, store->insert_atom_stmt, 12, atom->occupancy);
	sqlite3_bind_double_helper(store->db, store->insert_atom_stmt, 13, atom->tempFactor);
	sqlite3_bind_text_helper(store->db, store->insert_atom_stmt, 14, atom->element, -1);
	sqlite3_bind_text_helper(store->db, store->insert_atom_stmt, 15, atom->charge, -1);

	if (sqlite3_step(store->insert_atom_stmt) != SQLITE_DONE) {
		fprintf(stderr, "sqlite3: %s\n", sqlite3_errmsg(store->db));
		exit(1);
	}
	
	sqlite3_clear_bindings(store->insert_atom_stmt);
	sqlite3_reset(store->insert_atom_stmt);
}
