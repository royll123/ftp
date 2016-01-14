#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "ftp_common.h"

#define STAT_WAIT_CONNECT	1
#define STAT_WAIT_COMMAND	2

int main(int argc, char* argv[])
{
	int s, s2, stat, backlog = 5;
	struct sockaddr_in myskt;
	struct sockaddr_in skt;
	socklen_t sktlen = sizeof(skt);
	
	char pkt_data[sizeof(struct myftph_data)+1];
	char pkt[sizeof(struct myftph)+1];
	struct myftph header;
	struct myftph_data header_data;
	
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

	stat = STAT_WAIT_CONNECT;

	for(;;){
		switch(stat){
			case STAT_WAIT_CONNECT:
				if(listen(s, backlog) < 0){
					perror("listen");
					exit(1);
				}

				if((s2 = accept(s, (struct sockaddr*)&skt, &sktlen)) < 0){
					perror("accept");
					exit(1);
				}

				stat = STAT_WAIT_COMMAND;
				printf("changed\n");
				break;
			
			case STAT_WAIT_COMMAND:
				bzero(pkt, sizeof(pkt));
				if(recv(s2, pkt, sizeof(struct myftph), 0) < 0){
					perror("recv");
					exit(1);
				}
				read_ftp_packet(&header, pkt);
				break;
		}
	}

	if(close(s) < 0){
		perror("close");
		exit(1);
	}
}
