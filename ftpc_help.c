#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_help(int s, int argc, char* argv[])
{
	char commands[10][2][512] = 
		{	{"quit", "Close this connection.\n"},
			{"pwd", "Print the name of the current working directory on the remote machine.\n"},
			{"cd", "Move the current working directory on the remote machine.\n"},
			{"dir", "Print a list of the files on the current working directory on the remote machine.\n"},
			{"lpwd", "Print the name of the current working directory on the local machine.\n"},
			{"lcd", ""},
			{"ldir", ""},
			{"put", ""},
			{"get", ""}
		};
	int i;
	printf("Commands\n");
	for(i = 0; i < 10; i++){
		printf("%5s : %s\n", commands[i][0], commands[i][1]);
	}
}


