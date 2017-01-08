#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "pdb.h"

static void string_field(const char* src, char* dst, int start_col, int end_col) {
	int n = end_col - start_col + 1;
	strncpy(dst, src + start_col - 1, n);
	dst[n] = '\0';
	n = strtrimcpy(dst, dst);
	dst[n] = '\0';
}

static void char_field(const char* src, char *dst, int start_col) {	
	*dst = *(src + start_col - 1);
}

static void integer_field(const char* src, int *dst, int start_col, int end_col) {
	char aux[5];
	string_field(src, aux, start_col, end_col);
	*dst = atoi(aux);
}

static void real_field(const char* src, float *dst, int start_col, int end_col) {
	char aux[10];
	string_field(src, aux, start_col, end_col);
	*dst = strtod(aux, NULL);
}

PDBAtom* PDBAtom_create() {
	PDBAtom* atom = (PDBAtom*) malloc(sizeof(PDBAtom));
	if (atom == NULL) return NULL;
	atom->record_type[0] = '\0';
	atom->serial = 0;
	*(atom->name) = '\0';
	atom->altLoc = ' ';
	*(atom->resName) = '\0';
	atom->chainID = 'A';
	atom->resSeq = 0;
	atom->iCode = ' ';
	atom->x = 0;
	atom->y = 0;
	atom->z = 0;
	atom->occupancy = 0;
	atom->tempFactor = 0;
	*(atom->element) = '\0';
	*(atom->charge) = '\0';

	atom->next = NULL;
	return atom;
}

void PDBAtom_destroy(PDBAtom* atom) {
	free(atom);
}

void PDBAtom_parse(PDBAtom *atom, const char* str) {
	 string_field(str,   atom->record_type,  1,  6);
	integer_field(str, &(atom->serial)    ,  7, 11);
	 string_field(str,  (atom->name)      , 13, 16);
	   char_field(str, &(atom->altLoc)    , 17    );
	 string_field(str,  (atom->resName)   , 18, 20);
	   char_field(str, &(atom->chainID)   , 22    );
	integer_field(str, &(atom->resSeq)    , 23, 26);
	   char_field(str, &(atom->iCode)     , 27    );
	   real_field(str, &(atom->x)         , 31, 38);
	   real_field(str, &(atom->y)         , 39, 46);
	   real_field(str, &(atom->z)         , 47, 54);
	   real_field(str, &(atom->occupancy) , 55, 60);
	   real_field(str, &(atom->tempFactor), 61, 66);
	 string_field(str,  (atom->element)   , 77, 78);
	 string_field(str,  (atom->charge)    , 79, 80);
}

void PDBAtom_tostring(PDBAtom *atom, char* str) {
	char aux[5];
	if (strnlen(atom->name, 5) == 4) {
		sprintf(aux, "%s", atom->name);
	} else {
		sprintf(aux, " %s", atom->name);
	}
	aux[4] = '\0';	
	sprintf(str, "%-6s%5d %-4s%c%3s %c%4d%c   %8.3f%8.3f%8.3f%6.2f%6.2f          %2s%2s", atom->record_type, atom->serial, aux, atom->altLoc, atom->resName, atom->chainID, atom->resSeq, atom->iCode, atom->x, atom->y, atom->z, atom->occupancy, atom->tempFactor, atom->element, atom->charge);
}

void PDBAtom_print(PDBAtom *atom) {
	char s[81]; 
	s[80] = '\0';
	PDBAtom_tostring(atom, s);
	printf("%s\n", s);
}

/*
PDBModel* PDBModel_create() {
	PDBModel* model = (PDBModel*) malloc(sizeof(PDBModel));
	if (model == NULL) return NULL;
	model->serial = 0;
	model->first_atom = NULL;
	model->last_atom = NULL;
	return model;
}

void PDBModel_add(PDBModel* model, PDBAtom* atom) {
	if (model->first_atom == NULL) {
		model->first_atom = model->last_atom = atom;
	} else {
		model->last_atom->next = atom;
		model->last_atom = atom;
	}
}

void PDBModel_remove(PDBModel* model, PDBAtom* atom) {	
	if (model->first_atom == NULL) return;
	int cabeca = atom == model->first_atom;
	int cauda = atom == model->last_atom;
	if (cabeca || cauda) {
		if (cabeca) {
			model->first_atom = atom->next;
		}
		if (cauda) {
			model->last_atom = atom->next;
		}
	} else {
		PDBAtom* it = model->first_atom;
		while (it->next->next != NULL) {
			it = it->next;
		}
		if (it->next == atom) {
			it->next = atom->next;
		}
	}
}
*/