#include <stdio.h>
#include <netinet/in.h>
#include "ftp_common.h"


void create_ftp_packet(struct myftph* h, char *p, int size)
{
	snprintf(p, size, "%d%d%02d", h->type, h->code, h->length);
	printf("%s\n", p);
}

void read_ftp_packet(struct myftph* h, char *p)
{
	int i;
	uint8_t n[4];

	for(i = 0; i < 4; i++){
		n[i] = (uint8_t)(p[i]-'0');
	}
	h->type = n[0];
	h->code = n[1];
	h->length = n[2]*10+n[3];
}
