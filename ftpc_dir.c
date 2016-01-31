#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpc_commands.h"

void run_dir(int s, int argc, char* argv[])
{
	struct myftph header;
	char buf[HEADER_SIZE];
	char buf_data[HEADER_SIZE+DATASIZE];
	char data[DATASIZE+1];
	int pkt_size;
	bzero(&header, sizeof(header));
	bzero(buf, sizeof(buf));
	bzero(buf_data, sizeof(buf_data));
	bzero(data, sizeof(data));

	header.type = FTP_TYPE_CMD_LIST;
	if(argc == 2){
		send_data_packet(s, FTP_TYPE_CMD_LIST, 0x00, strlen(argv[1]), argv[1]);
	} else if(argc == 1){
		send_simple_packet(s, FTP_TYPE_CMD_LIST, 0x00);
	} else {
		fprintf(stderr, "Usage: dir [dir_path]\n");
		return;
	}

	if(recv(s, buf, HEADER_SIZE, 0) < 0){
		perror("recv");
		exit(1);
	}
	read_ftp_packet(&header, buf);

	if(header.type == FTP_TYPE_OK){
		while(1){
			bzero(buf_data, sizeof(buf_data));
			if(recv(s, buf, HEADER_SIZE, 0) < 0){
				perror("recv");
				close(s);
				exit(1);
			}
			read_ftp_packet(&header, buf);
			if(header.type == FTP_TYPE_DATA){
				if(recv(s, buf_data, header.length, 0) < 0){
					perror("recv");
					close(s);
					exit(1);
				}
				buf_data[header.length] = '\0';
				printf("%s\n", buf_data);
				if(header.code == 0x00) break;
			} else{
				fprintf(stderr, "Unexpected error occuered.\n");
				break;
			}
		}
	} else {
		output_error(&header);
	}
}


