#include <stdio.h>
#include <errno.h>
#include "ftp_common.h"
#include "ftpc_commands.h"

void output_error(struct myftph* h)
{
	switch(h->type){
		case FTP_TYPE_CMD_ERR:
			if(h->code == 0x01){
				fprintf(stderr, "Error: syntax error.\n");
			} else if(h->code == 0x02){
				fprintf(stderr, "Error: undefined command.\n");
			} else if(h->code == 0x03){
				fprintf(stderr, "Error: protocol error.\n");
			} else {
				fprintf(stderr, "program error.\n");
			}
			break;
		case FTP_TYPE_FILE_ERR:
			if(h->code == 0x00){
				fprintf(stderr, "Error: the file does not exist.\n");
			} else if(h->code == 0x01){
				fprintf(stderr, "Error: permission denied to access the file.\n");
			} else {
				fprintf(stderr, "program error.\n");
			}
			break;
		case FTP_TYPE_UNKWN_ERR:
			if(h->code = 0x05){
				fprintf(stderr, "Error: undefined error.\n");
			} else {
				fprintf(stderr, "program error.\n");
			}
			break;
		default:
			// this header may not be error.
			fprintf(stderr, "Protocol error.\n");
			break;
	}
}

void output_errno(int no)
{
	switch(no){
		case EACCES:
			fprintf(stderr, "Error: permission denied.\n");
			break;
		case ENOENT:
			fprintf(stderr, "Error: No such file or directory.\n");
			break;
		default:
			perror("");
			break;
	}
}
