#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpd_common.h"

void run_cd(int s, char* arg)
{
	struct myftph header;
	char buf[HEADER_SIZE];
	bzero(&header, sizeof(header));
	bzero(buf, sizeof(buf));

	// change current directory
	errno = 0;
	printf("path: %s\n", arg);
	if(chdir(arg) < 0){
		// error
		if(errno == ENOENT){
			header.type = FTP_TYPE_FILE_ERR;
			header.code = 0x00;
		} else if(errno == EACCES){
			header.type = FTP_TYPE_FILE_ERR;
			header.code = 0x01;
		} else {
			header.type = FTP_TYPE_UNKWN_ERR;
			header.code = 0x05;
		}
	} else {
		header.type = FTP_TYPE_OK;
	}
	
	create_ftp_packet(&header, buf);

	if(send(s, buf, HEADER_SIZE, 0) < 0){
		perror("send");
		exit(1);
	}
}


