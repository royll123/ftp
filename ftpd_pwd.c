#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpd_common.h"

void run_pwd(int s, char* arg)
{
	struct myftph header;
	char buf[HEADER_SIZE+DATASIZE];
	char path[DATASIZE];
	int pkt_size;
	bzero(&header, sizeof(header));

	// get current directory path
	if(getcwd(path, sizeof(path)) == NULL){
		header.type = FTP_TYPE_UNKWN_ERR;
		header.code = 0x05;
		create_ftp_packet(&header, buf);
		pkt_size = HEADER_SIZE;
	} else {
		header.type = FTP_TYPE_OK;
		header.length = strlen(path);
		create_ftp_packet_data(&header, path, buf);
		pkt_size = HEADER_SIZE + strlen(path);
	}

	if(send(s, buf, pkt_size, 0) < 0){
		perror("send");
		exit(1);
	}
}


