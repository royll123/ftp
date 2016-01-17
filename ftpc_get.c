#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpc_commands.h"

void send_offer(int, char*, int);
void get_dst_filename(char* src, char** dst);

void run_get(int s, int argc, char* argv[])
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

	printf("get\n");

	// set target files
	if(argc == 2){
		src = argv[1];
		get_dst_filename(src, &dst);
	} else if(argc == 3){
		src = argv[1];
		dst = argv[2];
	} else {
		fprintf(stderr, "Usage: get remote-file [local-file]\n");
		return;
	}

	printf("set target files.\n");

	// check create local file
	if((fd = open(dst, O_WRONLY|O_CREAT|O_TRUNC)) < 0){
		fprintf(stderr, "Error: Cannot create local file.\n");
		return;
	}

	send_offer(s, src, strlen(argv[1]));

	if(recv(s, buf, HEADER_SIZE, 0) < 0){
		perror("recv");
		close(fd);
		exit(1);
	}

	read_ftp_packet(&header, buf);
	printf("type:%d\n", header.type);
	printf("code: %d\n", header.code);

	if(header.type == FTP_TYPE_OK){
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
				//lseek(fd, header.length, SEEK_CUR);
				if(header.code == 0x00){
					break;
				}
			}
		}
	} else {
		// error
		fprintf(stderr, "error\n");
	}

	close(fd);
}

void send_offer(int s, char* src, int size)
{
	send_data_packet(s, FTP_TYPE_CMD_RETR, 0, size, src);
}

void get_dst_filename(char* src, char** dst)
{
	char* sp = src;
	while((*src) != '\0')
		src++;
	src--;

	printf("middle\n");
	while(src != sp && (*src) != '/')
		src--;

	printf("before last copy\n");

	(*dst) = src;
	printf("finished filename copy\n");
}