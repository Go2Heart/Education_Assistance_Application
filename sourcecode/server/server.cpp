#include "server.h"
#include "global.h"
#include "basicClass.h"
#include "customObject.h"
#include "identity.h"
#include "connect.h"
#include "graph.h"

void setSockNonBlock(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    if(flags < 0) {
        perror("fcntl(F_GETFL) failed");
    }
    if(fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("fcntl(F_SETFL) failed");
    }
}

Vector<Parameter> GetParms(String message) {
	int idx = 0, tot = 0, parmid = 0;
	Vector<Parameter> parms;
	while(idx < message.length() - 1) {
        String parm;
        int len = 0;
        bool isFile = false;
        if((unsigned char)message[idx] & 128) message[idx] -= 128, isFile = true;
        for(int j = idx; j <= idx + 3; j++)
            len = (len << 8) + (unsigned char)message[j];
        printf("parm id:%d len:%d\n", ++parmid, len);
        for(int j = idx + 4; j < idx + 4 + len; j++)
            parm.push_back(message[j]);
        idx = idx + 4 + len;
        parms.push_back(Parameter(parm, isFile));
    }
	return parms;
}

unsigned long long GetHash(String s) {
	unsigned long long tmp = 0;
	for(int i = 0; i < s.length(); i++)
		tmp = tmp * 10007 + s[i];
	return tmp;
}

void WriteFile(String path, String file) {
	FILE* out = fopen(path.c_str(), "wb");
	fwrite(file.c_str(), file.length() - 1, 1, out);
	fclose(out);
}

ssize_t socket_send(int sockfd, const char* buffer, size_t buflen) {
    ssize_t tmp;
    size_t total = buflen;
    const char* p = buffer;
    while(1) {
        tmp = send(sockfd, p, total, 0);
        if(tmp < 0) {
            //if(errno == EINTR) return -1;
            if(errno == EAGAIN) {
                usleep(500); continue;
            }
            return -1;
        }
        if((size_t)tmp == total) return buflen;
        total -= tmp;
        p += tmp;
    }
    return tmp;
}

bool sendAll(int &socket, Vector<Parameter> parms) {
    puts("begin send");
    char len[4];
    int totlen = 0;
    for(int i = 0; i < parms.size(); i++) {
        totlen += parms[i].s.length();
    }
    printf("totlen:%d\n",totlen);
    int nowsize = totlen;
    for(int i = 3; i >= 0; i--) {
        len[i] = nowsize % 256;
        nowsize >>= 8;
    }
    /*for(int i = 0; i <= 3; i++) {
        fprintf(stderr, "%d ", (int)len[i]);
    }*/
    socket_send(socket, len, 4);
    for(int i = 0; i < parms.size(); i++) {
        char buf[1024];
        for(int j = 0; j < parms[i].s.length(); j += 1024) {
            int nowlen = min(1024, (int)parms[i].s.length() - j);
            for(int k = 0; k < nowlen; k++) buf[k] = parms[i].s[j + k];
            socket_send(socket, buf, nowlen);
        }
    }
    puts("end send");
    return true;
}

Server::Server() {
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock == -1) puts("socket initialize error.");
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8888);

    int on = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setSockNonBlock(server_sock);
    if(bind(server_sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1)
        puts("bind error");
	if(listen(server_sock, 5) == -1)
        puts("listen error");
	puts("listen success");
}

void Server::run() {
    fd_set readfds;
    fd_set readfds_bak;
    timeval t = {2, 0};
    int maxfd = server_sock, res;
    FD_ZERO(&readfds);
    FD_ZERO(&readfds_bak);
    FD_SET(server_sock, &readfds_bak);
    int new_sock;
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char client_ip_str[INET_ADDRSTRLEN], buffer[BUF_SIZE];
    int recv_size;

    while(1) {
        
        readfds = readfds_bak;
        int new_maxfd = 0;
        for(int i = 0; i <= maxfd; i++) {
            if(FD_ISSET(i, &readfds) && i > new_maxfd) new_maxfd = i;
        }
        maxfd = new_maxfd;

        res = select(maxfd + 1, &readfds, NULL, NULL, &t);

        if(res == -1) {
            perror("select failed");
        } else if (res == 0) {
            //fprintf(stderr, "no socket ready for read");
            continue;
        }
        
        for(int i = 0; i <= maxfd; i++) {
            if(!FD_ISSET(i, &readfds)) continue;
            if(i == server_sock) {
                new_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_len);
                if(new_sock == -1) {
                    perror("accept failed");
                }
                if(!inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip_str, sizeof(client_ip_str))) {
                    perror("inet_ntop failed");//将IP转换为点数类型
                }
                printf("accept a client from: %s\n", client_ip_str);
                setSockNonBlock(new_sock);
                int on = 1;
                //setsockopt(new_sock, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));
                if(new_sock > maxfd) maxfd = new_sock;
                FD_SET(new_sock, &readfds_bak);
            } else {
                memset(buffer, 0, sizeof(buffer));
                recv(i, buffer, 4, 0);
                int Size = 0;
                for(int i = 0; i < 4; i++) {
                    Size = (Size << 8) + (unsigned char)buffer[i]; 
                }
                printf("Size: %d\n", Size);
                String message;
                while(Size > 0) {
                    if((recv_size = recv(i, buffer, sizeof(buffer), 0)) == -1) {
                        //perror("recv failed");
                        usleep(1000);
                        continue;
                    }
                    
                    //printf("left recv_size: %d\n", Size);
                    for(int i = 0; i < min(recv_size, Size); i++) message.push_back(buffer[i])/*, printf("%d ", (unsigned char)buffer[i])*/;
                    Size -= recv_size;
                }
                puts("finish recv");
                Vector<Parameter> parms = GetParms(message);
               /* switch (parms[0][0]) {
                    case 0x01 : {// 上传课程资料				
                        // lessonname filename file
                        Lesson* nowLesson = lessonGroup.GetLesson(parms[0]);
                        String savePath = "data/lesson_files/" + nowLesson->Name() + '/' + parms[1];
                        unsigned long long tmpHash = GetHash(parms[2]);
                        File* file = new File(savePath, tmpHash);
                        nowLesson->AddFile(file);
                        WriteFile(savePath, parms[2]);
                        break;
                    }
                    case 0x02 : {//提交作业
                        //studentId lessonid homeworkid filename file
                        Homework_Student* nowHomework = studentGroup.GetStudent(atoi(parms[0].c_str()))->Events()->GetLesson(atoi(parms[1].c_str()))->GetHomework(atoi(parms[2].c_str()));
                        String savePath = "data/student_files/" + parms[0] + "/lessons/" + parms[1] + '/' + parms[2] + '/' + ToString(nowHomework->Ver()) + '/' + parms[3];
                        nowHomework->Upload(savePath);
                        WriteFile(savePath, parms[3]);
                        break;
                    }
                    case 'a' : {*/
                        puts("getmessage");
                        sendAll(i, parms);
                        //sleep(100);
                        //break;
                    //}
                //}
                if(close(i) == -1) {
                    perror("close failed");
                }
                FD_CLR(i, &readfds_bak);
            }
        }
    }
}