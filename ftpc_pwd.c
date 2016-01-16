#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpc_commands.h"

void run_pwd(int s, int argc, char* argv[])
{
	struct myftph header;
	char buf[HEADER_SIZE+1];
	char buf_data[HEADER_SIZE+DATASIZE+1];
	char data[DATASIZE+1];
	bzero(&header, sizeof(header));
	bzero(buf, sizeof(buf));
	bzero(buf_data, sizeof(buf_data));
	bzero(data, sizeof(data));

	header.type = FTP_TYPE_CMD_PWD;
	create_ftp_packet(&header, buf);

	if(send(s, buf, HEADER_SIZE, 0) < 0){
		perror("send");
		exit(1);
	}

	if(recv(s, buf_data, HEADER_SIZE+DATASIZE, 0) < 0){
		perror("recv");
		exit(1);
	}
	read_ftp_packet_data(&header, buf_data, data);
	printf("type:%d\n", header.type);
	printf("length: %d\n", header.length);
	printf("data:%s\n", data);
}


