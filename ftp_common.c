#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <netinet/in.h>
#include "ftp_common.h"


void create_ftp_packet(struct myftph* h, char *p)
{
	int i, mask;
	uint8_t l_upper = h->length/0x100;
	p[0] = h->type;
	p[1] = h->code;
	p[2] = l_upper;
	p[3] = (uint8_t)(h->length%0x100);

//	for(i = 0; i < 4; i++){
//		printf("%02X", (unsigned char)p[i]);
//	}
//	printf("\n");
}

void create_ftp_packet_data(struct myftph* h, char *d, char *p)
{
	int i;
	create_ftp_packet(h, p);

	for(i = 0; i < h->length; i++){
		p[4+i] = d[i];
	}
}

void read_ftp_packet(struct myftph* h, char *p)
{
	int i;
	uint8_t n[4];

	for(i = 0; i < 4; i++){
//		printf("%02X", (unsigned char)p[i]);
		n[i] = (uint8_t)(p[i]);
	}
//	printf("\n");
	h->type = n[0];
	h->code = n[1];
	h->length = (uint16_t)n[2]*0x100+n[3];
}

void read_ftp_packet_data(struct myftph* h, char *p, char *d)
{
	int i;
	read_ftp_packet(h, p);

	for(i = 0; i < h->length; i++){
		*d++ = *(p+HEADER_SIZE);
		p++;
	}
	*d = '\0';
}

void send_simple_packet(int s, uint8_t type, uint8_t code)
{
	struct myftph header;
	char buf[HEADER_SIZE];
	bzero(&header, sizeof(header));
	header.type = type;
	header.code= code;
	create_ftp_packet(&header, buf);

	if(send(s, buf, HEADER_SIZE, 0) < 0){
		perror("send");
		exit(1);
	}
}

void send_data_packet(int s, uint8_t type, uint8_t code, uint16_t length, char* data)
{
	struct myftph header;
	char buf[HEADER_SIZE+DATASIZE];
	bzero(&header, sizeof(header));
	header.type = type;
	header.code= code;
	header.length = length;
	create_ftp_packet_data(&header, data, buf);

	if(send(s, buf, HEADER_SIZE+length, 0) < 0){
		perror("send");
		exit(1);
	}
}

