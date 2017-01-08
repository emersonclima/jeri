#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "getopt.h"
#include "dataset.h"

struct {
	bool create;
	bool verbose;
	char *dataset_filename;
	char **in_filenames;
	int in_filenames_count;
} global_args;

static const char* opt_string = "cvf:h?";

void display_usage() {
	printf("PDB Dataset By Emerson C. Lima\n");
	exit(EXIT_FAILURE);
}

void parse_global_args(int argc, char* argv[]) {
	int opt = getopt(argc, argv, opt_string);
    while (opt != -1) {
        switch (opt) {
            case 'c':
                global_args.create = true;
                break;                                
            case 'v':
                global_args.verbose = true;
                break;                
            case 'f':
                global_args.dataset_filename = optarg;
                break;  
            case 'h':   /* fall-through is intentional */
            case '?':
                display_usage();
                break;                
            default:
                /* You won't actually get here. */
                break;
        }
        
        opt = getopt(argc, argv, opt_string);
    }
    
    global_args.in_filenames = argv + optind;
    global_args.in_filenames_count = argc - optind;
}

int main(int argc, char* argv[]) {	
	global_args.create = false;
	global_args.verbose = false;
	global_args.dataset_filename = NULL;
	global_args.in_filenames_count = 0;

	parse_global_args(argc, argv);

	dataset* ds;
	dataset_open(global_args.dataset_filename, &ds);
	dataset_append_files(global_args.in_filenames, global_args.in_filenames_count, ds);

	//if (global_args.create) {
		//fprintf(stderr, "creating database...\n");		
		//for (int i = 0; i < global_args.in_filenames_count; i += 1) {
			//if (global_args.verbose) {
				//printf("[%2.0f%%] %s\n", (((float) i) / global_args.in_filenames_count) * 100, global_args.in_filenames[i]);			
			//}
			////dataset_append_file(global_args.in_filenames[i], ds);		
		//}
	//}
	
	dataset_close(ds);
}