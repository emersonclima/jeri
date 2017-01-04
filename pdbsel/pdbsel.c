#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "getopt.h"
#include "string.h"
#include "pdb.h"

struct args_t {
	FILE* in_file;
	FILE* out_file;
	bool verbose;
	bool reindex;

	bool is_atom;
	bool is_hetatm;
	char altloc_equals;
	char altloc_notequals;

	char resname_equals[4];
	char resname_notequals[4];
} global_args;

static const char *opt_string = "f:o:rvh?";

static const struct option longOpts[] = {
	{ "is-atom", 			no_argument, 		0, 0 },
	{ "is-hetatm", 			no_argument, 		0, 0 },
	{ "altloc-equals", 		required_argument, 	0, 0 },
	{ "altloc-not-equals", 	required_argument, 	0, 0 },
	{ "resname-equals", 	required_argument,  0, 0 },
	{ "resname-not-equals",	required_argument,  0, 0 }
};

void display_usage() {
	puts("pdbsel - Select pdb records");
	exit(EXIT_FAILURE);
}

void parse_global_args(int argc, char* argv[]) {
	global_args.in_file = stdin;
	global_args.out_file = stdout;
	global_args.verbose = false;
	global_args.reindex = false;
	global_args.is_atom = false;
	global_args.is_hetatm = false;

	strcpy(global_args.resname_equals, "");
	strcpy(global_args.resname_notequals, "");

	global_args.altloc_equals = 0;
	global_args.altloc_notequals = 0;

	int opt = 0;
	int longIndex = 0;
	opt = getopt_long(argc, argv, opt_string, longOpts, &longIndex);
	while (opt != -1) {
		switch(opt) {
			case 'f': {
				global_args.in_file = fopen(optarg, "r");
				if (global_args.in_file == NULL) {
					perror("Falha ao abrir arquivo para leitura");
					exit(EXIT_FAILURE);
				}
				break;
			}
			case 'o': {
				global_args.out_file = fopen(optarg, "w");
				if (global_args.out_file == NULL) {
					perror("Falha ao abrir arquivo para escrita");
					exit(EXIT_FAILURE);
				}
				break;
			}
			case 'v': global_args.verbose = true; break;
			case 'r': global_args.reindex = true; break;
			case 'h': case '?': display_usage(); break;
			default: break;
			case 0: {				
				if (strcmp("is-atom", longOpts[longIndex].name) == 0) {
					global_args.is_atom = true;
				} else if (strcmp("is-hetatm", longOpts[longIndex].name) == 0) {
					global_args.is_hetatm = true;
				} else if (strcmp("resname-equals", longOpts[longIndex].name) == 0) {
					if (strnlen(optarg, 4) != 3) {
						fprintf(stderr, "O comprimento do campo resname deve no máximo 3!\n");
						exit(EXIT_FAILURE);
					}
					strncpy(global_args.resname_equals, optarg, 4);
				} else if (strcmp("resname-not-equals", longOpts[longIndex].name) == 0) {
					if (strnlen(optarg, 4) != 3) {
						fprintf(stderr, "O comprimento do campo resname deve no máximo 3!\n");
						exit(EXIT_FAILURE);
					}
					strncpy(global_args.resname_notequals, optarg, 4);
				} else if (strcmp("altloc-equals", longOpts[longIndex].name) == 0) {
					if (strnlen(optarg, 2) != 1) {
						fprintf(stderr, "O comprimento do campo altLoc deve ser 1!\n");
						exit(EXIT_FAILURE);
					}
					global_args.altloc_equals = optarg[0];
				} else if (strcmp("altloc-not-equals", longOpts[longIndex].name) == 0) {
					if (strnlen(optarg, 2) != 1) {
						fprintf(stderr, "O comprimento do campo altLoc deve ser 1!\n");
						exit(EXIT_FAILURE);
					}
					global_args.altloc_notequals = optarg[0];
				}				
			}
		}
		opt = getopt_long(argc, argv, opt_string, longOpts, &longIndex);
	}
}

int main(int argc, char* argv[]) {
	parse_global_args(argc, argv);

	if (global_args.verbose) {
		if (global_args.is_atom) {
			fprintf(stderr, "is atom\n");			
		}
		if (global_args.is_hetatm) {
			fprintf(stderr, "is hetatm\n");
		}
		if (global_args.altloc_equals != 0) {
			fprintf(stderr, "altloc = %c\n", global_args.altloc_equals);
		}
		if (global_args.altloc_notequals != 0) {
			fprintf(stderr, "altloc != %c\n", global_args.altloc_notequals);
		}
		if (strnlen(global_args.resname_equals, 4) > 0) {
			fprintf(stderr, "resname = %s\n", global_args.resname_equals);
		}
		if (strnlen(global_args.resname_notequals, 4) > 0) {
			fprintf(stderr, "resname != %s\n", global_args.resname_equals);
		}
	}

	char buffer[81];
	PDBAtom atom;
	int serial = 1;
	while (fgets(buffer, 81, global_args.in_file) != NULL) {		
		bool is_atom = strstr(buffer, "ATOM   ") == buffer;		
		bool is_hetatm = strstr(buffer, "HETATM ") == buffer;
		
		if (!is_atom && !is_hetatm) continue;
		if (global_args.is_atom && !is_atom) continue;
		if (global_args.is_hetatm && !is_hetatm) continue;
		
		PDBAtom_parse(&atom, buffer);

		if (strnlen(global_args.resname_equals, 4) > 0 && strncmp(global_args.resname_equals, atom.resName, 3) != 0) continue;
		if (strnlen(global_args.resname_notequals, 4) > 0 && strncmp(global_args.resname_notequals, atom.resName, 3) == 0) continue;
		if (global_args.altloc_equals != 0 && atom.altLoc != global_args.altloc_equals) continue;
		if (global_args.altloc_notequals != 0 && atom.altLoc == global_args.altloc_notequals) continue;

		if (global_args.reindex) {
			atom.serial = serial;
		}

		PDBAtom_tostring(&atom, buffer);
		fprintf(global_args.out_file, "%s\n", buffer);

		serial += 1;
	}
	return EXIT_SUCCESS;
}
