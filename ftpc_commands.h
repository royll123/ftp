#ifndef FTPC_COMMANDS_H
#define FTPC_COMMANDS_H

#define STAT_INITIAL        0
#define STAT_WAIT_INPUT     1
#define STAT_WAIT_REPLY     2
#define STAT_QUIT			3

void run_quit(int, int, char*[]);
void run_pwd(int, int, char*[]);
void run_cd(int, int, char*[]);
void run_dir(int, int, char*[]);
void run_get(int, int, char*[]);
void run_put(int, int, char*[]);

void run_lpwd(int, int, char*[]);
void run_lcd(int, int, char*[]);
void run_ldir(int, int, char*[]);
void run_help(int, int, char*[]);
// fms
int stat;

#endif
