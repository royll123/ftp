#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "ftp_common.h"

#define STAT_INITIAL		0
#define STAT_WAIT_INPUT		1
#define STAT_WAIT_REPLY		2

int main(int argc, char* argv[])
{
	int s, stat;
	struct sockaddr_in skt;
	struct in_addr ipaddr;

	char pkt_data[sizeof(struct myftph_data)+1];
	char pkt[sizeof(struct myftph)+1];
	struct myftph header;
	struct myftph_data header_data;

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
	skt.sin_family = PF_INET;
	skt.sin_port = htons(FTP_PORT);
	skt.sin_addr.s_addr = htonl(ipaddr.s_addr);

	stat = STAT_INITIAL;

	for(;;){
		switch(stat){
			case STAT_INITIAL:
				if(connect(s, (struct sockaddr*)&skt, sizeof(skt)) < 0){
					perror("connect");
					exit(1);
				}

				stat = STAT_WAIT_INPUT;
				printf("changed\n");
				break;

			case STAT_WAIT_INPUT:
					bzero(&header, sizeof(header));
					header.type = 3;
					header.code = 2;
					create_ftp_packet(&header, pkt, sizeof(pkt));
					printf("send:%s\n", pkt);
					if(send(s, pkt, sizeof(struct myftph), 0) < 0){
						perror("send");
						exit(1);
					}
					sleep(100);
					exit(0);
				break;
		}
	}

	if(close(s) < 0){
		perror("close");
		exit(1);
	}
}
