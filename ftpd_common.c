#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include "ftp_common.h"
#include "ftpd_common.h"


void send_err_packet(int s, int no)
{
	switch(no){
		case EACCES:
			send_simple_packet(s, FTP_TYPE_FILE_ERR, 0x01);
			break;
		case EINTR:
			send_simple_packet(s, FTP_TYPE_FILE_ERR, 0x00);
			break;
		default:
			send_simple_packet(s, FTP_TYPE_UNKWN_ERR, 0x05);
			break;
	}
}
