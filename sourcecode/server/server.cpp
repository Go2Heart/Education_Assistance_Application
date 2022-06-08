#include "server.h"
#include "clock.h"
#include "global.h"
#include "basicClass.h"
#include "customObject.h"
#include "identity.h"
#include "graph.h"

int UnZip(String x) {
    int y = 0;
    for(int i = 0; i < x.length(); i++) y = (y << 8) + (unsigned char)x[i];
    return y;
}

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
    while(idx < message.length()) {
        String parm;
        int len = 0;
        int type = (unsigned char)message[idx];
        idx++;
        for(int j = idx; j <= idx + 3; j++)
            len = (len << 8) + (unsigned char)message[j];
        //printf("parm id:%d len:%d\n", ++parmid, len);
        for(int j = idx + 4; j < idx + 4 + len; j++)
            parm.push_back(message[j]);
        idx = idx + 4 + len;
        switch(type) {
            case 1:
                parms.push_back(Parameter(parm, false));
                break;
            case 2:
                parms.push_back(Parameter(parm, true));
                break;
            case 3:
                parms.push_back(Parameter(UnZip(parm)));
                break;
        }
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

bool sendAll(int &socket, Vector<Parameter> parms, bool debug) {
    if(debug) puts("begin send");
    char len[4];
    int totlen = 0;
    for(int i = 0; i < parms.size(); i++) {
        totlen += parms[i].s.length();
    }
    if(debug) printf("totlen:%d\n",totlen);
    int nowsize = totlen;
    for(int i = 3; i >= 0; i--) {
        len[i] = nowsize % 256;
        nowsize >>= 8;
    }
    for(int i = 0; i <= 3; i++) {
        if(debug) printf("%d ", (unsigned char)len[i]);
    }
    socket_send(socket, len, 4);
    for(int i = 0; i < parms.size(); i++) {
        char buf[1024];
        for(int j = 0; j < parms[i].s.length(); j += 1024) {
            int nowlen = min(1024, (int)parms[i].s.length() - j);
            for(int k = 0; k < nowlen; k++) {
                buf[k] = parms[i].s[j + k];
                if(debug) printf("%d ", (unsigned char)buf[k]);
            }
            socket_send(socket, buf, nowlen);
        }
    }
    if(debug) puts("");
    if(debug) puts("end send");
    return true;
}

Server::Server() {
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock == -1) puts("socket initialize error.");
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(43434);

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
                //printf("accept a client from: %s\n", client_ip_str);
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
                //printf("Size: %d\n", Size);
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
                //puts("finish recv");
                Vector<Parameter> parms = GetParms(message);
                if(parms.size() == 0) continue;
                switch (parms[0].number) {
                    case 0x01 : {// 上传课程资料
                        // lessonname filename file
                        /*Lesson* nowLesson = lessonGroup.GetLesson(parms[0]);
                        String savePath = "data/lesson_files/" + nowLesson->Name() + '/' + parms[1];
                        unsigned long long tmpHash = GetHash(parms[2]);
                        File* file = new File(savePath, tmpHash);
                        nowLesson->AddFile(file);
                        WriteFile(savePath, parms[2]);
                        break;*/
                        break;
                    }
                    case 0x02 : {//提交作业
                        //studentId lessonid homeworkid filename file
                        /*Homework_Student* nowHomework = studentGroup.GetStudent(atoi(parms[0].c_str()))->Events()->GetLesson(atoi(parms[1].c_str()))->GetHomework(atoi(parms[2].c_str()));
                        String savePath = "data/student_files/" + parms[0] + "/lessons/" + parms[1] + '/' + parms[2] + '/' + ToString(nowHomework->Ver()) + '/' + parms[3];
                        nowHomework->Upload(savePath);
                        WriteFile(savePath, parms[3]);
                        break;*/
                        break;
                    }
                    case 0x03 : {//查询路径
                        Vector<int> v;
                        int vectorSize = parms[1].number;
                        for(int j = 2; j < 2 + vectorSize; j++)
                            v.push_back(parms[j].number);
                        int queryMode = (unsigned char)parms[2 + vectorSize].number;
                        ResPackage result = graph.QueryDis(v, queryMode);
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(result.timeCost.Zip()));
                        resultParms.push_back(Parameter(result.v.size()));
                        for(int j = 0; j < result.v.size(); j++) {
                            resultParms.push_back(Parameter(result.v[j].type));
                            resultParms.push_back(Parameter(result.v[j].tool));
                            resultParms.push_back(Parameter(result.v[j].id));
                            printf("%d %d\n", result.v[j].type, result.v[j].id);
                        }
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x04 : {//查询时间
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(timeTracker.NowTimer().Zip()));
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x05 : {//修改速度
                        timeTracker.ChgSpd(parms[1].number / 10.0);
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(String("ack"), false));
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x06 : {
                        Vector<Parameter> resultParms;
                        int value;
                        if(parms[1].number == 1)
                            value = teacherGroup.GetTeacherCheck(parms[2].message, parms[3].message);
                        else
                            value = studentGroup.GetStudentCheck(parms[2].message, parms[3].message);
                        resultParms.push_back(Parameter(value));
                        sendAll(i, resultParms, false);
                        break;
                    }

                    case 0x07 : {//查询课程
                        Vector<Parameter> resultParms;
                        Vector<Lesson_Student*> lessons = studentGroup.GetStudent(parms[1].number)->events->lessons;
                        for(int j = 0; j < lessons.size(); j++) {
                            Lesson* nowLesson = lessonGroup.GetLesson(lessons[j]->lessonId);
                            resultParms.push_back(Parameter(nowLesson->Name(), false));
                            resultParms.push_back(Parameter(nowLesson->Teacher(), false));
                            //resultParms.push_back(Parameter(lessonGroup.GetLesson[i]->Time(), false));
                            resultParms.push_back(Parameter(nowLesson->Place(), false));
                            Vector<Duration> d = nowLesson->ClassDurations();
                            String time;
                            for(int i = 0; i < d.size(); i++) {
                                time = time + " " + ToString(d[i].begin.hour) + ':' + ToString(d[i].begin.minute) + '-' + ToString(d[i].end.hour) + ':' + ToString(d[i].end.minute) + ' ';
                            }
                            resultParms.push_back(Parameter(time, false));
                            resultParms.push_back(Parameter(lessonGroup.GetLesson(i)->QQ(), false));
                            resultParms.push_back(Parameter(ToString(i), false));// why not int?
                        }
                        sendAll(i, resultParms, false);
                        break;

                    }

                    case 0x08 : {//查询活动
                        Vector<Parameter> resultParms;
                        Vector<int> activities = studentGroup.GetStudent(parms[1].number)->events->activities;
                        printf("activity phase");
                        for(int j = 0; j < activities.size(); j++) {
                            Activity* nowActivity = activityGroup.GetActivity(activities[j]);
                            //resultParms.push_back(Parameter(i));
                            resultParms.push_back(Parameter(nowActivity->Name(), false));
                            //resultParms.push_back(Parameter(lessonGroup.GetLesson[i]->Time(), false));
                            resultParms.push_back(Parameter(nowActivity->Place(), false));
                            Duration d = nowActivity->Dura();
                            String time;
                            time = time + " " + ToString(d.begin.hour) + ':' + ToString(d.begin.minute) + '-' + ToString(d.end.hour) + ':' + ToString(d.end.minute) + ' ';
                            resultParms.push_back(Parameter(time, false));
                            resultParms.push_back(Parameter(ToString(activities[j]), false));
                        }
                        sendAll(i, resultParms, false);
                        break;

                    }
                    case 0x09 : {//检索活动ID
                        Vector<Parameter> resultParms;
                        int id = activityGroup.GetActivityId(parms[1].message);
                        resultParms.push_back(Parameter(id));
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x0A : {//检索活动
                        Vector<Parameter> resultParms;
                        int type = parms[2].number;
                        printf("%d\n", type);
                        printf("%s\n", parms[1].message.c_str());
                        switch(type) {
                            case 0: {//活动名称
                                Vector<Activity*> result = activityGroup.FromName(parms[1].message);
                                for(int j = 0; j < result.size(); j++) {
                                    resultParms.push_back(Parameter(result[j]->Name(), false));
                                    //resultParms.push_back(Parameter(lessonGroup.GetLesson[i]->Time(), false));
                                    resultParms.push_back(Parameter(result[j]->Place(), false));
                                    Duration d = result[j]->Dura();
                                    String time;
                                    time = time + " " + ToString(d.begin.hour) + ':' + ToString(d.begin.minute) + '-' + ToString(d.end.hour) + ':' + ToString(d.end.minute) + ' ';
                                    resultParms.push_back(Parameter(time, false));
                                    resultParms.push_back(Parameter(ToString(result[j]->activityId), false));
                                }
                                break;
                            }
                            case 1: {//活动地点
                                for(int j = 0; j < activityGroup.size(); j++) {
                                    Activity* nowActivity = activityGroup.GetActivity(j);
                                    if(nowActivity->Place()==(parms[1].message)) {
                                        resultParms.push_back(Parameter(nowActivity->Name(), false));
                                        //resultParms.push_back(Parameter(lessonGroup.GetLesson[i]->Time(), false));
                                        resultParms.push_back(Parameter(nowActivity->Place(), false));
                                        Duration d = nowActivity->Dura();
                                        String time;
                                        time = time + " " + ToString(d.begin.hour) + ':' + ToString(d.begin.minute) + '-' + ToString(d.end.hour) + ':' + ToString(d.end.minute) + ' ';
                                        resultParms.push_back(Parameter(time, false));
                                        resultParms.push_back(Parameter(ToString(j), false));
                                    }
                                }

                                break;
                            }
                            case 2: {//活动时间
                                const char* time = parms[1].message.c_str();
                                int index = 0;
                                int beginHour = 0;
                                int beginMin = 0;
                                int endHour = 0;
                                int endMin = 0;
                                while(time[index] != ':') {
                                    int tmp = time[index] - '0';
                                    beginHour = beginHour * 10 + tmp;
                                    index++;
                                }
                                index++;
                                while(time[index] != '\0') {
                                    int tmp = time[index] - '0';
                                    beginMin = beginMin * 10 + tmp;
                                    index++;
                                }
                                Timer begin(beginHour, beginMin);

                                for(int j = 0; j < activityGroup.size(); j++) {
                                    Activity* nowActivity = activityGroup.GetActivity(j);
                                    if(nowActivity->Dura().begin.HMLessEqual(begin) &&
                                        begin.HMLessEqual(activityGroup.GetActivity(j)->Dura().end)) {
                                        resultParms.push_back(Parameter(nowActivity->Name(), false));
                                        //resultParms.push_back(Parameter(lessonGroup.GetLesson[i]->Time(), false));
                                        resultParms.push_back(Parameter(nowActivity->Place(), false));
                                        Duration d = nowActivity->Dura();
                                        String time;
                                        time = time + " " + ToString(d.begin.hour) + ':' + ToString(d.begin.minute) + '-' + ToString(d.end.hour) + ':' + ToString(d.end.minute) + ' ';
                                        resultParms.push_back(Parameter(time, false));
                                        resultParms.push_back(Parameter(ToString(j), false));
                                    }
                                }

                                break;
                            }

                        }
                        sendAll(i, resultParms, true);
                        break;

                    }
                    case 0x0B : {//活动上传
                        Student* nowStudent = studentGroup.GetStudent(parms[5].number);
                        Vector<Student*> nowStudents;
                        nowStudents.push_back(nowStudent);
                        const char* time = parms[4].message.c_str();
                        int index = 0;
                        int beginHour = 0;
                        int beginMin = 0;
                        int endHour = 0;
                        int endMin = 0;
                        while(time[index] != ':') {
                            int tmp = time[index] - '0';
                            beginHour = beginHour * 10 + tmp;
                            index++;
                        }
                        index++;
                        while(time[index] != '-') {
                            int tmp = time[index] - '0';
                            beginMin = beginMin * 10 + tmp;
                            index++;
                        }
                        index++;
                        while(time[index] != ':') {
                            int tmp = time[index] - '0';
                            endHour = endHour * 10 + tmp;
                            index++;
                        }
                        index++;
                        while(time[index] != '\0') {
                            int tmp = time[index] - '0';
                            endMin = endMin * 10 + tmp;
                            index++;
                        }
                        printf("%d %d %d %d\n", beginHour, beginMin, endHour, endMin);

                        Duration tmpDuration = Duration(
                            Timer(beginHour, beginMin),
                            Timer(endHour,endMin)
                        );
                        Activity* nowActivity = new Activity(parms[1].message, parms[2].message, parms[3].number, tmpDuration,nowStudents);
                        int activityID = activityGroup.AddActivities(nowActivity);
                        studentGroup.GetStudent(parms[5].number)->events->AddActivity(activityID);
                        break;
                    }
                    case 0x0C : {//活动文件上传
                        int id = parms[1].number;
                        Activity* nowActivity = activityGroup.GetActivity(id);
                        String savePath = "../Activity/" + ToString(id) + "/" + parms[2].message;
                        unsigned long long tmpHash = GetHash(parms[3].message);
                        File* file = new File(savePath, tmpHash);
                        nowActivity->AddFile(file);
                        WriteFile(savePath, parms[3].message);
                        break;
                    }
                    case 0x0D : {//课程作业上传 参数：学生ID 课程ID 作业下标 文件名 文件内容
                        Homework_Student* nowHomework = studentGroup.
                            GetStudent(parms[1].number)->
                                events->
                                    GetLesson(parms[2].number)->
                                        GetHomework(parms[3].number);
                        
                        String savePath = "../Lesson/"
                            + ToString(parms[1].number) + "/"
                            + ToString(parms[2].number) + "/" 
                            + ToString(parms[3].number) + "/"
                            + parms[4].message;
                        printf("%s\n", savePath.c_str());
                        unsigned long long tmpHash = GetHash(parms[5].message);
                        File* file = new File(savePath, tmpHash);
                        nowHomework->Upload(file);
                        WriteFile(savePath, parms[5].message);
                        break;
                    }
                    case 0x0E : { // 添加作业 参数： 课程ID 作业描述
                        lessonGroup.GetLesson(parms[1].number)->AddHomework(new Homework(parms[2].message));
                        break;
                    }
                }
                if(close(i) == -1) {
                    perror("close failed");
                }
                FD_CLR(i, &readfds_bak);
            }
        }
    }
}