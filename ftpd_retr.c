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


void retr_err_handler(int, int);

void run_retr(int s, char* arg)
{
	struct myftph header;
	char buf[HEADER_SIZE+DATASIZE];
	char data[DATASIZE];
	size_t pkt_size;
	int fd;
	bzero(&header, sizeof(header));
	bzero(data, sizeof(data));

	// get directory path
	if(arg == NULL){
		send_simple_packet(s, FTP_TYPE_CMD_ERR, 0x01);
		return;
	}

	printf("get directory path:%s\n", arg);

	// open directory
	errno = 0;
	if((fd = open(arg, O_RDONLY)) < 0){
		// error
		retr_err_handler(s, errno);
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
			//	lseek(fd, pkt_size, SEEK_CUR);
			}
		}
	}

	if(close(fd) < 0){
		perror("close");
		exit(1);
	}
}

void retr_err_handler(int s, int no)
{
	if(no == EACCES){
		// permission denied
		send_simple_packet(s, FTP_TYPE_FILE_ERR, 0x01);
	} else if(no == ENOENT){
		// directory does not exist
		send_simple_packet(s, FTP_TYPE_FILE_ERR, 0x00);
	} else {
		// unknown error
		send_simple_packet(s, FTP_TYPE_UNKWN_ERR, 0x05);
	}
}
