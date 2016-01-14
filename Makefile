
SERVER=ftpd.c
CLIENT=ftpc.c

all: server client

server: $(SERVER)
	gcc $(SERVER) -o ftpd

client: $(CLIENT)
	gcc $(CLIENT) -o ftpc

clean:
	rm -rf ftpd ftpc
