#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "ftp_common.h"
#include "ftpc_commands.h"

struct command_table{
	char *cmd;
	void (*func)(int, int, char**);
}cmd_tbl[] = {
	{"quit",	run_quit},
	{"pwd",		run_pwd},
	{"cd",		run_cd},
	{"dir",		run_dir},
	{"lpwd",	NULL},
	{"lcd",		NULL},
	{"ldir",	NULL},
	{"get",		run_get},
	{"put",		NULL},
	{"help",	NULL},
	{NULL,		NULL}
};

int find_func(char*);

void getargs(int*, char*[], char*);

int main(int argc, char* argv[])
{
	int s;
	struct sockaddr_in skt;
	struct in_addr ipaddr;

	char pkt_data[sizeof(struct myftph_data)+1];
	char pkt[sizeof(struct myftph)+1];
	struct myftph header;
	struct myftph_data header_data;

	char input[216];
	int agc, func;
	char *agv[24];

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
	skt.sin_addr.s_addr = ipaddr.s_addr;

	stat = STAT_INITIAL;

	for(;;){
		switch(stat){
			case STAT_INITIAL:
				if(connect(s, (struct sockaddr*)&skt, sizeof(skt)) < 0){
					perror("connect");
					exit(1);
				}

				stat = STAT_WAIT_INPUT;
				break;

			case STAT_WAIT_INPUT:
				printf("myFTP%%");
				bzero(input, sizeof(input));
				if(fgets(input, sizeof(input), stdin) == NULL){
					putchar('\n');
					return 0;
				}
				input[strlen(input)-1] = '\0';
				if(*input == '\0') break;
				getargs(&agc, agv, input);
				func = find_func(agv[0]);
				if(cmd_tbl[func].cmd == NULL){
					fprintf(stderr, "command not found: %s\n", agv[0]);
				} else {
					cmd_tbl[func].func(s, agc, agv);
				}
				/*bzero(&header, sizeof(header));
					header.type = 3;
					header.code = 2;
					create_ftp_packet(&header, pkt, sizeof(pkt));
					printf("send:%s\n", pkt);
					if(send(s, pkt, sizeof(struct myftph), 0) < 0){
						perror("send");
						exit(1);
					}
					sleep(100);
					exit(0);*/
				break;

			case STAT_QUIT:
				if(close(s) < 0){
					perror("close");
					exit(1);
				}
				exit(0);
		}
	}

	if(close(s) < 0){
		perror("close");
		exit(1);
	}
}

int find_func(char* cmd)
{
	int i = 0;
	while(cmd_tbl[i].cmd != NULL){
		if(strcmp(cmd_tbl[i].cmd,cmd) == 0){
			break;
		}
		i++;
	}

	return i;
}

void getargs(int* argc, char* argv[], char* p)
{
	*argc = 0;

	while(1){
		while(*p == ' ' || *p == '\t' || *p == '\n')
			p++;
		if(*p == '\0') break;

		argv[(*argc)++] = p;

		while(*p != ' ' && *p != '\t' && *p != '\n' && *p != '\0')
			p++;
		if(*p == '\0') break;
		*p++ = '\0';
	}
	argv[(*argc)] = NULL;
}
