#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "ftp_common.h"


int main(int argc, char* argv[])
{
	int s, backlog = 5;
	struct sockaddr_in myskt;
	struct sockaddr_in skt;
	socklen_t sktlen = sizeof(skt);
	// socket
	if((s = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	}
	bzero(&myskt, sizeof(myskt));
	myskt.sin_family = AF_INET;
	myskt.sin_port = htons(FTP_PORT);
	myskt.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(s, (struct sockaddr*)&myskt, sizeof(myskt)) < 0){
		perror("bind");
		exit(1);
	}

	if(listen(s, backlog) < 0){
		perror("listen");
		exit(1);
	}

	if((s = accept(s, (struct sockaddr*)&skt, &sktlen)) < 0){
		perror("accept");
		exit(1);
	}

	printf("connected!");

	close(s);
}
