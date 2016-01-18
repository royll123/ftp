#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpd_common.h"

void run_pwd(int s, char* arg)
{
	char path[DATASIZE];

	if(arg != NULL){
		send_simple_packet(s, FTP_TYPE_CMD_ERR, 0x01);
		return;
	}

	// get current directory path
	errno = 0;
	if(getcwd(path, sizeof(path)) == NULL){
		send_err_packet(s, errno);
	} else {
		send_data_packet(s, FTP_TYPE_OK, 0x00, strlen(path), path);
	}
}


