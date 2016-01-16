#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpc_commands.h"

void run_dir(int s, int argc, char* argv[])
{
	struct myftph header;
	char buf[HEADER_SIZE+DATASIZE];
	char buf_data[HEADER_SIZE+DATASIZE];
	char data[DATASIZE+1];
	int pkt_size;
	bzero(&header, sizeof(header));
	bzero(buf, sizeof(buf));
	bzero(buf_data, sizeof(buf_data));
	bzero(data, sizeof(data));

	header.type = FTP_TYPE_CMD_LIST;
	if(argc == 2){
		header.length = strlen(argv[1]);
		create_ftp_packet_data(&header, argv[1], buf);
		pkt_size = HEADER_SIZE+header.length;
	} else if(argc == 1){
		create_ftp_packet(&header, buf);
		pkt_size = HEADER_SIZE;
	} else {
		fprintf(stderr, "Usage: dir [dir_path]\n");
		return;
	}

	if(send(s, buf, pkt_size, 0) < 0){
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


