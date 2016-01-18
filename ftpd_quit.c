#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpd_common.h"

void run_quit(int s, char* arg)
{
	struct myftph header;
	char buf[HEADER_SIZE];
	bzero(&header, sizeof(header));

	header.type = FTP_TYPE_OK;
	create_ftp_packet(&header, buf);

	if(send(s, buf, HEADER_SIZE, 0) < 0){
		perror("send");
		exit(1);
	}

	state = STAT_QUIT;
}


