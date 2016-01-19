#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpc_commands.h"

void run_cd(int s, int argc, char* argv[])
{
	struct myftph header;
	char buf[HEADER_SIZE+1];
	bzero(&header, sizeof(header));
	bzero(buf, sizeof(buf));


	// check args
	if(argc != 2){
		fprintf(stderr, "Usage: cd <target>\n");
		return;
	}

	send_data_packet(s, FTP_TYPE_CMD_CWD, 0x00, strlen(argv[1]), argv[1]);

	if(recv(s, buf, HEADER_SIZE, 0) < 0){
		perror("recv");
		exit(1);
	}
	read_ftp_packet(&header, buf);
	
	if(header.type != FTP_TYPE_OK){
		output_error(&header);
	}
}


