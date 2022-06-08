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
    int type;
    int number;
    String s;
    String message;
    Parameter() {}
    Parameter(String m, bool isfile) : type(isfile ? 2 : 1) {
        message = m;
        int totlen = message.length();
        char len[4];
        for(int i = 3; i >= 0; i--) len[i] = totlen % 256, totlen >>= 8;
        s.push_back(type);
        for(int i = 0; i <= 3; i++) s.push_back(len[i]);
        s = s + message;
        puts("create parameter finished.");
    }
    Parameter(int x) : type(3), number(x) {
        if(x != 0) {
            int st[105], top = 0;
            while(x) {
                st[++top] = x % 256;
                x >>= 8;
            }
            for(int i = top; i > 0; i--) message.push_back(st[i]);
        } else message.push_back(0);
        int totlen = message.length();
        char len[4];
        for(int i = 3; i >= 0; i--) len[i] = totlen % 256, totlen >>= 8;
        s.push_back(type);
        for(int i = 0; i <= 3; i++) s.push_back(len[i]);
        s = s + message;
    }
    Parameter(const Parameter& para) {
        type = para.type;
        number = para.number;
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