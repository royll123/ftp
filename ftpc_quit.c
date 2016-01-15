#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpc_commands.h"

void run_quit(int s, int argc, char* argv[])
{
	struct myftph header;
	char buf[HEADER_SIZE+1];
	bzero(&header, sizeof(header));

	header.type = FTP_TYPE_CMD_QUIT;
	create_ftp_packet(&header, buf, sizeof(buf));

	if(send(s, buf, HEADER_SIZE, 0) < 0){
		perror("send");
		exit(1);
	}
}


