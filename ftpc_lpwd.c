#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "ftpd_common.h"

void run_lpwd(int s, int argc, char* argv[])
{
	char path[256];

	// check arguments
	if(argc != 1){
		fprintf(stderr, "lpwd: too many arguments\n");
		return;
	}

	// get current directory path
	if(getcwd(path, sizeof(path)) == NULL){
		fprintf(stderr, "Error: failed to run pwd\n");
	} else {
		printf("%s\n", path);
	}
}


