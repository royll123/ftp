#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "ftp_common.h"
#include "ftpd_common.h"


void err_handler(int, int);

void run_list(int s, char* arg)
{
	struct myftph header;
	char buf[HEADER_SIZE+DATASIZE];
	char data[DATASIZE];
	char tmp_path[DATASIZE];
	char *path = NULL;
	int pkt_size;
	DIR *dir;
	struct dirent *file;
	bzero(&header, sizeof(header));
	bzero(data, sizeof(data));

	// get directory path
	errno = 0;
	if(arg == NULL){
		if(getcwd(tmp_path, sizeof(tmp_path)) == NULL){
			send_err_packet(s, errno);
		} else {
			path = tmp_path;
		}
	} else {
		path = arg;
	}

	// open directory
	if(path != NULL){
		if((dir = opendir(path)) == NULL){
			send_err_packet(s, errno);
		} else {
			char* p_data = data;
			char line[DATASIZE];
			int rest = DATASIZE;
			struct stat st;
			struct tm *t;

			// send ok reply
			send_simple_packet(s, FTP_TYPE_OK, 0x00);

			// create list and send it
			while((file = readdir(dir)) != NULL){
				bzero(line, sizeof(line));
				if(stat(file->d_name, &st) < 0){
					continue;
				}
				t = localtime(&st.st_mtime);
				sprintf(line, "%u %lu %d:%d %5ld %2d/%2d %02d:%02d %s", st.st_mode, st.st_nlink, 
						st.st_uid, st.st_gid, st.st_size, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, file->d_name);

				if(strlen(line)+1 > rest){
					// send data block
					p_data--;
					(*p_data) = '\0';
					send_data_packet(s, FTP_TYPE_DATA, 0x01, strlen(data), data);
					p_data = data;
					(*p_data) = '\0';
					rest = DATASIZE;
				}

				strncpy(p_data, line, rest);
				rest -= strlen(line)+1;
				p_data += strlen(line);
				(*p_data++) = '\n';
			}
			if(p_data != data){
				p_data--;
				(*p_data) = '\0';
			}
			send_data_packet(s, FTP_TYPE_DATA, 0x00, strlen(data), data);
		}
	}
}
