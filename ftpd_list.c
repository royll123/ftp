#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpd_common.h"


void err_handler(int, struct myftph*);

void run_list(int s, char* arg)
{
	struct myftph header;
	char buf[HEADER_SIZE+DATASIZE];
	char data[DATASIZE];
	char tmp_path[DATASIZE];
	char *path = NULL;
	int pkt_size;
	DIR *dir;
	struct dirent *file;
	bzero(&header, sizeof(header));
	bzero(data, sizeof(data));

	// get directory path
	errno = 0;
	if(arg == NULL){
		if(getcwd(tmp_path, sizeof(tmp_path)) == NULL){
			err_handler(errno, &header);
			create_ftp_packet(&header, buf);
			pkt_size = HEADER_SIZE;
		} else {
			path = tmp_path;
		}
	} else {
		path = arg;
	}

	printf("get directory path:%s\n", path);

	// open directory
	if(path != NULL){
		if((dir = opendir(path)) == NULL){
			err_handler(errno, &header);
			create_ftp_packet(&header, buf);
			pkt_size = HEADER_SIZE;
		} else {
			char* p_data = data;
			int rest = DATASIZE;
			while((file = readdir(dir)) != NULL){
				strncpy(p_data, file->d_name, rest);
				rest -= strlen(file->d_name)+1;
				p_data += strlen(file->d_name);
				(*p_data++) = '\n';
			}
			if(p_data != data){
				p_data--;
				(*p_data) = '\0';
			}
			header.type = FTP_TYPE_OK;
			header.length = strlen(data);
			create_ftp_packet_data(&header, data, buf);
			pkt_size = HEADER_SIZE + header.length;
		}
	}

	if(send(s, buf, pkt_size, 0) < 0){
		perror("send");
		exit(1);
	}
}

void err_handler(int no, struct myftph* header)
{
	if(no == EACCES){
		// permission denied
		header->type = FTP_TYPE_FILE_ERR;
		header->code = 0x01;
	} else if(no == ENOENT){
		// directory does not exist
		header->type = FTP_TYPE_FILE_ERR;
	} else {
		// unknown error
		header->type = FTP_TYPE_UNKWN_ERR;
		header->code = 0x05;
	}
}
