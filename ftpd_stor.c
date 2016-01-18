#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpd_common.h"

void run_stor(int s, char* arg)
{
	struct myftph header;
	char buf[HEADER_SIZE+DATASIZE];
	char buf_data[HEADER_SIZE+DATASIZE];
	char data[DATASIZE+1];
	char *src, *dst;
	int fd, pkt_size;
	bzero(&header, sizeof(header));
	bzero(buf, sizeof(buf));
	bzero(buf_data, sizeof(buf_data));
	bzero(data, sizeof(data));

	if(arg == NULL){
		send_simple_packet(s, FTP_TYPE_CMD_ERR, 0x01);
		return;
	}

	// check create local file
	errno = 0;
	if((fd = open(arg, O_WRONLY|O_CREAT|O_TRUNC)) < 0){
		send_err_packet(s, errno);
		return;
	}

	// send ok message
	send_simple_packet(s, FTP_TYPE_OK, 0x00);

	// receive file data
	while(1){
		if(recv(s, buf_data, HEADER_SIZE+DATASIZE, 0) < 0){
			perror("recv");
			close(fd);
			exit(1);
		}
		read_ftp_packet_data(&header, buf_data, data);
		if(header.type == FTP_TYPE_DATA){
			if(write(fd, data, header.length) < 0){
				perror("write");
				close(fd);
				exit(1);
			}
			if(header.code == 0x00){
				break;
			}
		} else {
			// protocol error
			send_simple_packet(s, FTP_TYPE_CMD_ERR, 0x03);
			break;
		}
	}	

	if(close(fd) < 0){
		perror("close");
		exit(1);
	}
}
