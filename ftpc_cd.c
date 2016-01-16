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
	char buf_data[HEADER_SIZE+DATASIZE+1];
	char res[HEADER_SIZE];
	bzero(&header, sizeof(header));
	bzero(buf, sizeof(buf));
	bzero(buf_data, sizeof(buf_data));


	// check args
	if(argc != 2){
		fprintf(stderr, "Usage: cd <target>\n");
		return;
	}

	header.type = FTP_TYPE_CMD_CWD;
	header.length = strlen(argv[1]);
	create_ftp_packet_data(&header, argv[1], buf_data);

	if(send(s, buf_data, HEADER_SIZE+header.length, 0) < 0){
		perror("send");
		exit(1);
	}

	if(recv(s, buf, HEADER_SIZE, 0) < 0){
		perror("recv");
		exit(1);
	}

	read_ftp_packet(&header, buf);
	printf("type:%d\n", header.type);
	printf("length: %d\n", header.length);
}


