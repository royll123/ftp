#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "ftp_common.h"


int main(int argc, char* argv[])
{
	int s;
	struct sockaddr_in skt;
	struct in_addr ipaddr;

	if(inet_pton(AF_INET, argv[1], &ipaddr) != 1){
		fprintf(stderr, "wrong server_ip_address\n");
		exit(1);
	}

	// socket
	if((s = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	}

	bzero(&skt, sizeof(skt));
	skt.sin_family = AF_INET;
	skt.sin_port = htons(FTP_PORT);
	skt.sin_addr.s_addr = htonl(ipaddr.s_addr);

	if(connect(s, (struct sockaddr*)&skt, sizeof(skt)) < 0){
		perror("connect");
		exit(1);
	}

	printf("connected!");

	close(s);
}
