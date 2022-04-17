#ifndef CONNECT_H
#define CONNECT_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "data_structure/string.h"
using namespace std;

class Socket {
private:
    int server_sock;
    int client_sock;
    struct sockaddr_in server_addr;
    //struct sockaddr_in client_addr;
public:
    Socket();
    String receiveMessage();
    void sendMessage(String message);
    void closeSocket();
};

#endif