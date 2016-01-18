#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpd_common.h"

void run_quit(int s, char* arg)
{
	send_simple_packet(s, FTP_TYPE_OK, 0x00);

	state = STAT_QUIT;
}


