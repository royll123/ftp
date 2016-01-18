#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_help(int s, int argc, char* argv[])
{
	char commands[10][2][512] = 
		{	{"quit", "Close this connection."},
			{"pwd", "Print the name of the current working directory on the remote machine."},
			{"cd", "Move the current working directory on the remote machine."},
			{"dir", "Print a list of the files on the current working directory on the remote machine."},
			{"lpwd", "Print the name of the current working directory on the local machine."},
			{"lcd", "Move the curent working directory on the local machine."},
			{"ldir", "Print a list of the files on the current working directory on the local machine."},
			{"put", "Store a local file on the remote machine."},
			{"get", "Retrive the remote file and store it on the local machine."}
		};
	int i;
	printf("Commands\n");
	for(i = 0; i < 10; i++){
		printf("%5s : %s\n", commands[i][0], commands[i][1]);
	}
}


