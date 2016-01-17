#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>

void run_lcd(int s, int argc, char* argv[])
{
	// check arguments
	if(argc != 2){
		fprintf(stderr, "Usage: lcd DIR\n");
		return;
	}
	
	// change current directory
	errno = 0;
	if(chdir(argv[1]) < 0){
		// error
		if(errno == ENOENT){
		} else if(errno == EACCES){
		} else {
		}
	}
}


