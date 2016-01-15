#ifndef FTP_COMMON_H
#define FTP_COMMON_H

#include <stdint.h>
#include <netinet/in.h>


// FTP HEADER TYPE LIST
#define FTP_TYPE_CMD_QUIT	0x01
#define FTP_TYPE_CMD_PWD	0x02
#define FTP_TYPE_CMD_CWD	0x03
#define FTP_TYPE_CMD_LIST	0x04
#define FTP_TYPE_CMD_RETR	0x05
#define FTP_TYPE_CMD_STOR	0x06

#define FTP_TYPE_OK			0x10
#define FTP_TYPE_CMD_ERR	0x11
#define FTP_TYPE_FILE_ERR	0x12
#define FTP_TYPE_UNKWN_ERR	0x13

#define FTP_TYPE_DATA		0x20


#define FTP_PORT	50021

#define HEADER_SIZE	4
#define DATASIZE	1024

struct myftph {
	uint8_t type;
	uint8_t code;
	uint16_t length;
};

struct myftph_data {
	uint8_t type;
	uint8_t code;
	uint16_t length;
	char data[DATASIZE];
};

void create_ftp_packet(struct myftph*, char*, int);
void read_ftp_packet(struct myftph*, char*);
#endif
