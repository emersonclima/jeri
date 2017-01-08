#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "getopt.h"
#include "string.h"
#include "pdb.h"
#include "ast.h"

//provided by flex and bison tools
void yy_scan_string(const char *str);
int yyparse (void* tag);
int yylex_destroy();

struct args_t {
	FILE* in_file;
	FILE* out_file;
	bool verbose;
	bool reindex;
	char* filter_exp;

	bool is_atom;
	bool is_hetatm;
	char altloc_equals;
	char altloc_notequals;

	char resname_equals[4];
	char resname_notequals[4];
} global_args;

static const char *opt_string = "f:o:e:rvh?";

void display_usage() {
	puts("pdbfilter - Filter PDB records");
	exit(EXIT_FAILURE);
}

void parse_global_args(int argc, char* argv[]) {
	global_args.in_file = stdin;
	global_args.out_file = stdout;
	global_args.filter_exp = "true";
	global_args.verbose = false;
	global_args.reindex = false;	

	int opt = 0;
	int longIndex = 0;
	opt = getopt(argc, argv, opt_string);
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
			case 'e': global_args.filter_exp = strdup(optarg); break;
			default: break;			
		}
		opt = getopt(argc, argv, opt_string);
	}
}

ASTNode *parse_filter(const char* s) {
	yy_scan_string(s);
	ASTNode *aux = NULL;
    yyparse((void*) &aux);
    yylex_destroy();        
    return aux;

}

bool eval_node(ASTNode* node) {
	if (node == NULL) return false;
	if (node->type == AST_BOOLEAN) return node->value.bval;
	if (node->type == AST_AND) return eval_node(node->left) && eval_node(node->right);
	if (node->type == AST_OR) return eval_node(node->left) || eval_node(node->right);
	if (node->type == AST_NOT) return !eval_node(node->left);
	return false;
}

int main(int argc, char* argv[]) {	
	parse_global_args(argc, argv);	
	ASTNode* ast = parse_filter(global_args.filter_exp);
	print_dot(ast);
	bool r = eval_node(ast);
	printf("%s\n", r ? "true" : "false");

	return 0;

	char buffer[81];
	PDBAtom atom;
	int serial = 1;
	while (fgets(buffer, 81, global_args.in_file) != NULL) {		
		bool is_atom = strstr(buffer, "ATOM   ") == buffer;		
		bool is_hetatm = strstr(buffer, "HETATM ") == buffer;
		
		if (!is_atom && !is_hetatm) continue;
		
		PDBAtom_parse(&atom, buffer);

		if (global_args.reindex) {
			atom.serial = serial;
		}

		PDBAtom_tostring(&atom, buffer);
		fprintf(global_args.out_file, "%s\n", buffer);

		serial += 1;
	}
	return EXIT_SUCCESS;
}
