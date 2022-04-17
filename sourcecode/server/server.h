#ifndef SERVER_H
#define SERVER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include "data_structure/string.h"
#include "data_structure/vector.h"
using namespace std;

#define BUF_SIZE 1024

struct Parameter {
    bool isFile;
    String s;
    String message;
    Parameter() {}
    Parameter(String m, bool isfile) : isFile(isfile) {
        message = m;
        int totlen = message.length();
        char len[4];
        for(int i = 3; i >= 0; i--) len[i] = totlen % 256, totlen >>= 8;
        if(isFile) len[0] += 128;
        for(int i = 0; i <= 3; i++) s.push_back(len[i]);
        s = s + message;
        puts("create parameter finished.");
    }
    Parameter(const Parameter& para) {
        isFile = para.isFile;
        s = para.s;
        message = para.message;
    }
};

class Server {
private:
    int server_sock;
    Vector<int> clients;
    sockaddr_in server_addr;
public:
    Server();
    void run();
};

#endif