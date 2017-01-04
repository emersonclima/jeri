#ifndef _JERI_H_
#define _JERI_H_

#include "util.h"

struct pdb_atom {
	char record_type[7];
	int serial;
	char name[5];
	char altLoc;
	char resName[4];
	char chainID;
	int resSeq;
	char iCode;
	float x;
	float y;
	float z;
	float occupancy;
	float tempFactor;
	char element[3];
	char charge[3];

	struct pdb_atom* next;
};
typedef struct pdb_atom PDBAtom;

PDBAtom* PDBAtom_create();
void PDBAtom_parse(PDBAtom *atom, const char* str);
void PDBAtom_tostring(PDBAtom *atom, char* dst);
void PDBAtom_print(PDBAtom *atom);
void PDBAtom_destroy();

/*
struct pdb_model {
	int serial;
	PDBAtom* first_atom;
	PDBAtom* last_atom;
};
typedef struct pdb_model PDBModel;

PDBModel* PDBModel_create();
void PDBModel_destroy();
void PDBModel_add(PDBModel* model, PDBAtom* atom);
void PDBModel_remove(PDBModel* model, PDBAtom* atom);

//int PDBModel_addAtom(PDBAtom* atom);
//int PDBModel_remove(PDBAtom* atom);
//
//int PDBFile_create(PDBFile* pdb);
//int PDBFile_parse(PDBFile* pdb, char* filename);
//int PDBFile_addModel(PDBFile*);
//int PDBFile_getModel(PDBFile*, int serial);
//void PDBFile_destroy(PDBFile* pdb);
*/
#endif