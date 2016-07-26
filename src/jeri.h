#ifndef _JERI_H_
#define _JERI_H_

int strtrimcpy(char* dst, const char* src);
int parse_lines(const char* filename, int buffer_size, int (*callback) (const char* line, int count));

struct pdb_atom {
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
};
typedef struct pdb_atom PDBAtom;

void PDBAtom_parse(PDBAtom *atom, const char* str);
void PDBAtom_tostring(PDBAtom *atom, char* dst);
void PDBAtom_print(PDBAtom *atom);
/*
struct atom;
typedef struct atom Atom;
int atom_parse(char* s, Atom* a);
*/
#endif