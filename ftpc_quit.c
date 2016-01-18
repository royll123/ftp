#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpc_commands.h"

void run_quit(int s, int argc, char* argv[])
{
	send_simple_packet(s, FTP_TYPE_CMD_QUIT, 0x00);

	state = STAT_QUIT;
}


