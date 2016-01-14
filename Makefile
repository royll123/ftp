COMMON=ftp_common.c
SERVER=ftpd.c
CLIENT=ftpc.c

all: server client

server: $(SERVER) $(COMMON)
	gcc $(SERVER) $(COMMON) -o ftpd

client: $(CLIENT) $(COMMON)
	gcc $(CLIENT) $(COMMON) -o ftpc

clean:
	rm -rf ftpd ftpc
