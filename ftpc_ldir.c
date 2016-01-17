#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>


void ldir_err_handler(int);

void run_ldir(int s, int argc, char* argv[])
{
	char tmp_path[512];
	char *path = NULL;
	DIR *dir;
	struct dirent *file;

	// check arguments
	if(argc > 2){
		fprintf(stderr, "Usage: ldir [DIR]\n");
		return;
	} else if(argc == 1){
		// set path
		errno = 0;
		if(getcwd(tmp_path, sizeof(tmp_path)) == NULL){
			ldir_err_handler(errno);
		} else {
			path = tmp_path;
		}
	} else {
		path = argv[1];
	}

	// open directory
	if(path != NULL){
		if((dir = opendir(path)) == NULL){
			ldir_err_handler(errno);
		} else {
			while((file = readdir(dir)) != NULL){
				printf("%s\n", file->d_name);
			}
		}
	}
}

void ldir_err_handler(int no)
{
	if(no == EACCES){
		// permission denied
	} else if(no == ENOENT){
		// directory does not exist
	} else {
		// unknown error
	}
}
