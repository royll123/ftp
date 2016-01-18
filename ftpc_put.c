#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpc_commands.h"


extern void get_dst_filename(char* src, char** dst);

void run_put(int s, int argc, char* argv[])
{
	struct myftph header;
	char buf[HEADER_SIZE];
	char data[DATASIZE];
	size_t pkt_size;
	char *src, *dst;
	int fd;
	bzero(&header, sizeof(header));
	bzero(data, sizeof(data));

	// get directory path
	if(argc == 2){
		src = argv[1];
		get_dst_filename(src, &dst);
	} else if(argc == 3){
		src = argv[1];
		dst = argv[2];
	} else {
		fprintf(stderr, "Usage: put local_file [server_file]\n");
		return;
	}

	// open directory
	errno = 0;
	if((fd = open(src, O_RDONLY)) < 0){
		// error
		output_errno(errno);
		return;
	}

	send_data_packet(s, FTP_TYPE_CMD_STOR, 0x00, strlen(dst), dst);
	

	// get reply
	if(recv(s, buf, HEADER_SIZE, 0) < 0){
		perror("recv");
		close(fd);
		exit(1);
	}
	read_ftp_packet(&header, buf);

	if(header.type == FTP_TYPE_OK){
		// send data
		while(1){
			if((pkt_size = read(fd, data, DATASIZE)) < DATASIZE){
				// last block
				send_data_packet(s, FTP_TYPE_DATA, 0x00, pkt_size, data);
				break;
			} else {
				// continue to send
				send_data_packet(s, FTP_TYPE_DATA, 0x01, pkt_size, data);
			}
		}
	} else {
		output_error(&header);
	}

	if(close(fd) < 0){
		perror("close");
		exit(1);
	}
}
