COMMON=ftp_common.c
SERVER_FILE=ftpd.c
CLIENT_FILE=ftpc.c ftpc_quit.c

SERVER=myftpd
CLIENT=myftpc

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_FILE) $(COMMON)
	gcc $(SERVER_FILE) $(COMMON) -o $(SERVER)

$(CLIENT): $(CLIENT_FILE) $(COMMON)
	gcc $(CLIENT_FILE) $(COMMON) -o $(CLIENT)

clean:
	rm -rf $(SERVER) $(CLIENT)
