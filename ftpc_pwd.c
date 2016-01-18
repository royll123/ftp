#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpc_commands.h"

void run_pwd(int s, int argc, char* argv[])
{
	struct myftph header;
	char buf_data[HEADER_SIZE+DATASIZE+1];
	char data[DATASIZE+1];
	bzero(&header, sizeof(header));
	bzero(buf_data, sizeof(buf_data));
	bzero(data, sizeof(data));

	send_simple_packet(s, FTP_TYPE_CMD_PWD, 0x00);

	if(recv(s, buf_data, HEADER_SIZE+DATASIZE, 0) < 0){
		perror("recv");
		exit(1);
	}
	read_ftp_packet_data(&header, buf_data, data);

	if(header.type == FTP_TYPE_OK){
		printf("%s\n", data);
	} else {
		output_error(&header);
	}
}


