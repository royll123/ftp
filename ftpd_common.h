#ifndef FTPD_COMMON_H
#define FTPD_COMMON_H

#define STAT_WAIT_CONNECT   1
#define STAT_WAIT_COMMAND   2
#define STAT_QUIT			3

// fms
int stat;


void run_quit(int, char*);
void run_pwd(int, char*);
void run_cd(int, char*);
void run_list(int, char*);
void run_retr(int, char*);


#endif
