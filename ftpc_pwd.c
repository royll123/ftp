#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpc_commands.h"

void run_pwd(int s, int argc, char* argv[])
{
	struct myftph header;
	char buf[HEADER_SIZE];
	char data[DATASIZE+1];
	bzero(&header, sizeof(header));
	bzero(buf, sizeof(buf));
	bzero(data, sizeof(data));

	send_simple_packet(s, FTP_TYPE_CMD_PWD, 0x00);

	if(recv(s, buf, HEADER_SIZE, 0) < 0){
		perror("recv");
		exit(1);
	}
	read_ftp_packet(&header, buf);

	if(header.type == FTP_TYPE_OK){
		if(recv(s, data, header.length, 0) < 0){
			perror("recv");
			exit(1);
		}
		data[header.length] = '\0';
		printf("%s\n", data);
	} else {
		output_error(&header);
	}
}


