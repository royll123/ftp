#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpd_common.h"

void run_cd(int s, char* arg)
{
	if(arg == NULL){
		send_simple_packet(s, FTP_TYPE_CMD_ERR, 0x01);
		return;
	}

	// change current directory
	errno = 0;
	if(chdir(arg) < 0){
		send_err_packet(s, errno);
	} else {
		send_simple_packet(s, FTP_TYPE_OK, 0x00);
	}
}


