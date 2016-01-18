#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpd_common.h"

void run_retr(int s, char* arg)
{
	char data[DATASIZE];
	size_t pkt_size;
	int fd;
	bzero(data, sizeof(data));

	// get directory path
	if(arg == NULL){
		send_simple_packet(s, FTP_TYPE_CMD_ERR, 0x01);
		return;
	}

	// open directory
	errno = 0;
	if((fd = open(arg, O_RDONLY)) < 0){
		// error
		send_err_packet(s, errno);
		return;
	} else {
		// ok reply
		send_simple_packet(s, FTP_TYPE_OK, 0x01);

		// get_data
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
	}

	if(close(fd) < 0){
		perror("close");
		exit(1);
	}
}
