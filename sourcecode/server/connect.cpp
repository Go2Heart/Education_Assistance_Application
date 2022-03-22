#include "connect.h"

Socket::Socket() {
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock == -1) puts("socket initialize error.");
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8888);

    int on = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(bind(server_sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1)
        puts("bind error");
	if(listen(server_sock, 5) == -1)
        puts("listen error");
	puts("listen success");
}

string Socket::receiveMessage() {
    client_sock = accept(server_sock, NULL, NULL);
    puts("accept success");
    int idx = 0, read_len = 0, str_len = 0;
    string message;
    char x;
    while(read_len = read(client_sock, &x, 1))
        message.push_back(x);
	return message;
}

void Socket::sendMessage(string message) {
	write(client_sock, message.c_str(), message.length());
}

void Socket::closeSocket() {
	close(client_sock);
}