#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpd_common.h"

struct command_table{
	int type;
	void (*func)(int, char*);
}cmd_tbl[] = {
	{FTP_TYPE_CMD_QUIT,	run_quit},
	{FTP_TYPE_CMD_PWD,	run_pwd},
	{FTP_TYPE_CMD_CWD,	run_cd},
	{FTP_TYPE_CMD_LIST, run_list},
	{FTP_TYPE_CMD_RETR,	run_retr},
	{FTP_TYPE_CMD_STOR,	run_stor},
	{-1, NULL}
};

int s, s2;

int getcmd(int);
void close_handler(int);
void set_signal();

int main(int argc, char* argv[])
{
	int r, backlog = 5;
	struct sockaddr_in myskt;
	struct sockaddr_in skt;
	socklen_t sktlen = sizeof(skt);
	
	char pkt_data[HEADER_SIZE+DATASIZE+1];
	char pkt[HEADER_SIZE+1];
	struct myftph header;
	char ftp_data[DATASIZE+1];
	
	set_signal();

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
				} else {
					pid_t pid = fork();
					if(pid < 0){
						perror("fork");
						exit(1);
					} else if(pid == 0){
						stat = STAT_WAIT_COMMAND;
					} else {
						
					}
				}
				
				break;
			
			case STAT_WAIT_COMMAND:
			{
				int cmd_type;
				bzero(pkt_data, sizeof(pkt_data));
				if((r = recv(s2, pkt_data, HEADER_SIZE+DATASIZE, 0)) < 0){
					perror("recv");
					close(s2);
					exit(1);
				}
				read_ftp_packet_data(&header, pkt_data, ftp_data);
				cmd_type = getcmd(header.type);
				if(cmd_tbl[cmd_type].type == -1){
					// invalid type of ftp header
					send_simple_packet(s2, FTP_TYPE_CMD_ERR, 0x03);
				} else {
					if(header.length == 0){
						cmd_tbl[cmd_type].func(s2, NULL);
					} else {
						cmd_tbl[cmd_type].func(s2, ftp_data);
					}
				}
				break;
			}

			case STAT_QUIT:
				if(close(s2) < 0){
					perror("close");
					exit(1);
				}
				exit(0);
				break;
		}
	}

	if(close(s) < 0){
		perror("close");
		exit(1);
	}
}

int getcmd(int type)
{
	int i = 0;
	while(cmd_tbl[i].type != -1){
		if(cmd_tbl[i].type == type)
			break;
		else
			i++;
	}
	return i;
}

void close_handler(int sig)
{
	if(close(s2) < 0){
		perror("close");
		exit(1);
	}
	
	if(close(s) < 0){
		perror("close");
		exit(1);
	}
}

void set_signal()
{
	struct sigaction act;
	act.sa_handler = &close_handler;
	act.sa_flags = 0;

	if(sigaction(SIGINT, &act, NULL) < 0){
		perror("sigaction");
		exit(1);
	}
}
