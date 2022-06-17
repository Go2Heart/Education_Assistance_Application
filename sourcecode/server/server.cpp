#include "server.h"
#include "clock.h"
#include "global.h"
#include "basicClass.h"
#include "customObject.h"
#include "identity.h"
#include "graph.h"
#include "timeHeap.h"
#include "huffman/decoder.h"
#include "huffman/encoder.h"
#include "sha256.h"

Encoder encodeSolver;
Decoder decodeSolver;

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

void WriteFile(String path, String file) {
    String tmppath;
    for(int i = 0; i < path.size(); i++) {
        if(path[i] == '/') mkdir(tmppath.c_str(), 0777);
        tmppath.push_back(path[i]);
    }
    FILE* tmpout = fopen("tmpfile", "wb");
    fwrite(file.c_str(), file.length(), 1, tmpout);
    fclose(tmpout);
    tmpout = fopen("tmpfile", "rb");
    FILE* out = fopen(path.c_str(), "wb");
    encodeSolver.Encode(tmpout, out, 0, 0);
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

void Log(String s) {
    fprintf(logFile, "%s\n", s.c_str());
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
    int maxfd = server_sock, res, local = 0;
    FD_ZERO(&readfds);
    FD_ZERO(&readfds_bak);
    FD_SET(server_sock, &readfds_bak);
    FD_SET(0, &readfds_bak);
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
            if(i == local) {
                char opt[233];
                scanf("%s", opt);
                if(string(opt) == string("exit")) {
                    fclose(logFile);
                    return;
                }
            } else if(i == server_sock) {
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
                    case 0x01 : {// 添加课程作业 updated
                        Teacher* nowTeacher = teacherGroup.GetTeacher(parms[1].number);
                        Lesson* nowLesson = lessonGroup.GetLesson(parms[2].number);
                        Log("[添加作业]  教师 " + nowTeacher->name + " 对课程 " + nowLesson->Name() + "添加了作业。");
                        nowLesson->AddHomework(new Homework(parms[3].message));
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(String("ack"), false));
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x02 : {// 课程作业上传 参数：学生ID 课程ID 作业下标 文件个数 多个文件
                        Student* nowStudent = studentGroup.GetStudent(parms[1].number);
                        Lesson_Student* nowLesson = nowStudent->events->GetLesson(parms[2].number);
                        Homework_Student* nowHomework = nowLesson->GetHomework(parms[3].number);
                        Log("[提交作业]  学生 " + nowStudent->name + " 对课程 " + lessonGroup.GetLesson(nowLesson->lessonId)->Name() + " 提交了 " + nowHomework->desc + " 作业");
                        Vector<File*> fileVector;
                        bool success = true;
                        for(int j = 0, k = 5; j < parms[4].number; j++, k += 2) {
                            String savePath = "HomeWork/"
                            + ToString(parms[1].number) + "/"
                            + ToString(parms[2].number) + "/" 
                            + ToString(parms[3].number) + "/"
                            + parms[k].message;
                            String tmpHash = sha256(parms[k + 1].message);
                            success &= homeworkFiles.verifyFile(tmpHash);
                            File* file = new File(savePath, parms[k].message, tmpHash);
                            fileVector.push_back(file);
                            WriteFile(savePath, parms[k + 1].message);
                        }
                        nowHomework->Upload(fileVector);
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(success));
                        sendAll(i, resultParms, false);
                        break;
                        break;
                    }
                    case 0x03 : { /* // case changed
                                   * 查询作业
                                   * params: 学生ID
                                   * params: 课程ID
                                   * return: 作业ID & 作业状态 &作业描述
                                   */
                        Vector<Parameter> resultParms;
                        Student* nowStudent = studentGroup.GetStudent(parms[1].number);
                        Lesson_Student* nowLesson = nowStudent->events->GetLesson(parms[2].number);
                        Vector<Homework_Student*> homework = nowLesson->HomeworkStates();
                        Log("[查询作业集合] 学生 " + nowStudent->name + " 查询了课程 " + lessonGroup.GetLesson(nowLesson->lessonId)->Name() + " 的作业集合。");
                        resultParms.push_back(Parameter(homework.size()));
                        for (int i = 0; i < homework.size(); i++) {
                            resultParms.push_back(Parameter(homework[i]->id));
                            resultParms.push_back(Parameter(homework[i]->finished));
                            resultParms.push_back(Parameter(homework[i]->desc, false));
                        }
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x04 : {
                        /**
                         * @brief 搜索作业
                         * params: 学生id
                         * params: 课程id
                         * params: 作业名称
                         */
                        Student* nowStudent = studentGroup.GetStudent(parms[1].number);
                        Lesson_Student* nowLesson = nowStudent->events->GetLesson(parms[2].number);
                        Vector<Homework_Student*> homework = nowLesson->FromHomeworkName(parms[3].message);
                        Log("[检索作业集合] 学生 " + nowStudent->name + " 检索了课程 " + lessonGroup.GetLesson(nowLesson->lessonId)->Name() + " 的作业集合，关键词为 " + parms[3].message + "。");
                        Vector<Parameter> resultParms;
                        for (int i = 0; i < homework.size(); i++) {
                            resultParms.push_back(Parameter(homework[i]->id));
                            resultParms.push_back(Parameter(homework[i]->finished));
                            resultParms.push_back(Parameter(homework[i]->desc, false));
                        }
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x05 : {//查询路径
                        String log;
                        Student* nowStudent = studentGroup.GetStudent(parms[1].number);
                        Vector<int> v;
                        int vectorSize = parms[2].number;
                        for(int j = 3; j < 3 + vectorSize; j++)
                            v.push_back(parms[j].number);
                        int queryMode = (unsigned char)parms[3 + vectorSize].number;
                        ResPackage result = graph.QueryDis(v, queryMode);

                        Vector<String> modeString;
                        modeString.push_back("最短距离");
                        modeString.push_back("最短时间");
                        modeString.push_back("交通工具的最短时间");
                        log = log + "[查询路径] 学生 " + nowStudent->name + " 查询了一条路径 策略为 " + modeString[queryMode - 1];
                        log = log + "起始点为 " + graph.GetPoint(parms[3].number).Name() + " 终点为" + graph.GetPoint(parms[2 + vectorSize].number).Name();
                        log = log + "途经点序列为：";
                        for(int j = 4; j < 2 + vectorSize; j++) {
                            log = log + " " + graph.GetPoint(parms[3].number).Name();
                        }
                        log = log + "。";
                        Log(log);

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
                    case 0x06 : {//查询时间
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(timeTracker.NowTimer().Zip()));
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x07 : {//修改速度 update
                        if(parms[1].number == 0) {
                            Student* nowStudent = studentGroup.GetStudent(parms[2].number);
                            Log("[修改时间速度] 学生 " + nowStudent->name + " 修改了时间速度。");
                        } else {
                            Teacher* nowTeacher = teacherGroup.GetTeacher(parms[2].number);
                            Log("[修改时间速度] 教师 " + nowTeacher->name + " 修改了时间速度。");
                        }
                        timeTracker.ChgSpd(parms[3].number / 10.0);
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(String("ack"), false));
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x08 : { // 登录
                        Vector<Parameter> resultParms;
                        int value;
                        if(parms[1].number == 1) {
                            value = teacherGroup.GetTeacherCheck(parms[2].message, parms[3].message);
                            if(value != 255) {
                                Log("[登录请求] 老师 " + teacherGroup.GetTeacher(value)->name + " 已登录。");
                            }
                        }
                        else {
                            value = studentGroup.GetStudentCheck(parms[2].message, parms[3].message);
                            if(value != 255) {
                                Log("[登录请求] 学生 " + studentGroup.GetStudent(value)->name + " 已登录。");
                                if(!ClockStatus) timeTracker.restart();
                            }
                        }
                        resultParms.push_back(Parameter(value));
                        sendAll(i, resultParms, false);
                        break;
                    }

                    case 0x09 : {//查询课程
                        Vector<Parameter> resultParms;
                        if(parms[1].number == 0) {
                            Student* nowStudent = studentGroup.GetStudent(parms[2].number);
                            Log("[课程列表查询] 学生 " + nowStudent->name + " 查询了课程列表。");
                            Vector<Lesson_Student*> lessons = nowStudent->events->lessons;
                            for(int j = 0; j < lessons.size(); j++) {
                                Lesson* nowLesson = lessonGroup.GetLesson(lessons[j]->lessonId);
                                resultParms.push_back(Parameter(nowLesson->Name(), false));
                                resultParms.push_back(Parameter(nowLesson->Teacher(), false));
                                //resultParms.push_back(Parameter(lessonGroup.GetLesson[i]->Time(), false));
                                resultParms.push_back(Parameter(nowLesson->Place(), false));
                                Vector<Duration> d = nowLesson->ClassDurations();
                                String time;
                                for(int k = 0; k < d.size(); k++) {
                                    time = time + ToString(d[k].begin.day) + " "  + ToString_Time(d[k].begin.hour) + ':' + ToString_Time(d[k].begin.minute) + '-' + ToString_Time(d[k].end.hour) + ':' + ToString_Time(d[k].end.minute) + ' ';
                                }
                                resultParms.push_back(Parameter(time, false));
                                resultParms.push_back(Parameter(nowLesson->QQ(), false));
                                resultParms.push_back(Parameter(lessons[j]->lessonId));// why not int?
                                Vector<File*> files = nowLesson->Files();
                                resultParms.push_back(Parameter(files.size()));
                                for(int k = 0; k < files.size(); k++) {
                                    resultParms.push_back(Parameter(files[k]->name, false));
                                }
                                Duration exam = nowLesson->ExamDuration();
                                int b1 = exam.begin.Zip();
                                resultParms.push_back(Parameter(b1));
                                int e1 = exam.end.Zip();
                                resultParms.push_back(Parameter(e1));
                                resultParms.push_back(Parameter(nowLesson->ExamPlace(), false));
                            }
                        } else {
                            Teacher* nowTeacher = teacherGroup.GetTeacher(parms[2].number);
                            Log("[课程列表查询] 老师 " + nowTeacher->name + " 查询了课程列表。");
                            Vector<int> lessons = nowTeacher->lessons;
                            for(int j = 0; j < lessons.size(); j++) {
                                Lesson* nowLesson = lessonGroup.GetLesson(lessons[j]);
                                resultParms.push_back(Parameter(nowLesson->Name(), false));
                                resultParms.push_back(Parameter(nowLesson->Teacher(), false));
                                //resultParms.push_back(Parameter(lessonGroup.GetLesson[i]->Time(), false));
                                resultParms.push_back(Parameter(nowLesson->Place(), false));
                                Vector<Duration> d = nowLesson->ClassDurations();
                                String time;
                                for(int k = 0; k < d.size(); k++) {
                                    time = time + ToString(d[k].begin.day) + " "  + ToString_Time(d[k].begin.hour) + ':' + ToString_Time(d[k].begin.minute) + '-' + ToString_Time(d[k].end.hour) + ':' + ToString_Time(d[k].end.minute) + ' ';
                                }
                                resultParms.push_back(Parameter(time, false));
                                resultParms.push_back(Parameter(nowLesson->QQ(), false));
                                resultParms.push_back(Parameter(lessons[j]));// why not int?
                                Vector<File*> files = nowLesson->Files();
                                resultParms.push_back(Parameter(files.size()));
                                for(int k = 0; k < files.size(); k++) {
                                    resultParms.push_back(Parameter(files[k]->name, false));
                                }
                                Duration exam = nowLesson->ExamDuration();
                                int b1 = exam.begin.Zip();
                                resultParms.push_back(Parameter(b1));
                                int e1 = exam.end.Zip();
                                resultParms.push_back(Parameter(e1));
                                resultParms.push_back(Parameter(nowLesson->ExamPlace(), false));
                            }
                        }
                        
                        sendAll(i, resultParms, false);
                        break;
                    }

                    case 0x0A : { // update
                        /*
                         * 检索课程
                         * param: studentId key
                         * return: Vector<Lesson>
                         */
                        Vector<Parameter> resultParms;

                        if(parms[1].number == 0) {
                            Student* nowStudent = studentGroup.GetStudent(parms[2].number);
                            Log("[课程列表检索] 学生 " + nowStudent->name + " 检索了课程列表，关键词为 " + parms[3].message + " 。");
                            Vector<Lesson*> result = nowStudent->events->FromLessonName(parms[3].message);
                            for(int j = 0; j < result.size(); j++) {
                                Lesson *nowLesson = result[j];
                                resultParms.push_back(Parameter(nowLesson->Name(), false));
                                resultParms.push_back(Parameter(nowLesson->Teacher(), false));
                                //resultParms.push_back(Parameter(lessonGroup.GetLesson[i]->Time(), false));
                                resultParms.push_back(Parameter(nowLesson->Place(), false));
                                Vector<Duration> d = nowLesson->ClassDurations();
                                String time;
                                for(int k = 0; k < d.size(); k++) {
                                    time = time + ToString(d[k].begin.day) + " " + ToString_Time(d[k].begin.hour) + ':' + ToString_Time(d[k].begin.minute) + '-' + ToString_Time(d[k].end.hour) + ':' + ToString_Time(d[k].end.minute) + ' ';
                                }
                                resultParms.push_back(Parameter(time, false));
                                resultParms.push_back(Parameter(nowLesson->QQ(), false));
                                resultParms.push_back(Parameter(ToString(j), false));// why not int?
                                Vector<File*> files = nowLesson->Files();
                                resultParms.push_back(Parameter(files.size()));
                                for(int k = 0; k < files.size(); k++) {
                                    resultParms.push_back(Parameter(files[k]->name, false));
                                }
                                Duration exam = nowLesson->ExamDuration();
                                int b1 = exam.begin.Zip();
                                resultParms.push_back(Parameter(b1));
                                int e1 = exam.end.Zip();
                                resultParms.push_back(Parameter(e1));
                                resultParms.push_back(Parameter(nowLesson->ExamPlace(), false));
                            }
                        } else {
                            Teacher* nowTeacher = teacherGroup.GetTeacher(parms[2].number);
                            Log("[课程列表检索] 老师 " + nowTeacher->name + " 检索了课程列表，关键词为 " + parms[3].message + " 。");
                            Vector<Lesson*> result = nowTeacher->FromLessonName(parms[3].message);
                            for(int j = 0; j < result.size(); j++) {
                                Lesson *nowLesson = result[j];
                                resultParms.push_back(Parameter(nowLesson->Name(), false));
                                resultParms.push_back(Parameter(nowLesson->Teacher(), false));
                                //resultParms.push_back(Parameter(lessonGroup.GetLesson[i]->Time(), false));
                                resultParms.push_back(Parameter(nowLesson->Place(), false));
                                Vector<Duration> d = nowLesson->ClassDurations();
                                String time;
                                for(int k = 0; k < d.size(); k++) {
                                    time = time + ToString(d[k].begin.day) + " " + ToString_Time(d[k].begin.hour) + ':' + ToString_Time(d[k].begin.minute) + '-' + ToString_Time(d[k].end.hour) + ':' + ToString_Time(d[k].end.minute) + ' ';
                                }
                                resultParms.push_back(Parameter(time, false));
                                resultParms.push_back(Parameter(nowLesson->QQ(), false));
                                resultParms.push_back(Parameter(ToString(j), false));// why not int?
                                Vector<File*> files = nowLesson->Files();
                                resultParms.push_back(Parameter(files.size()));
                                for(int k = 0; k < files.size(); k++) {
                                    resultParms.push_back(Parameter(files[k]->name, false));
                                }
                                Duration exam = nowLesson->ExamDuration();
                                int b1 = exam.begin.Zip();
                                resultParms.push_back(Parameter(b1));
                                int e1 = exam.end.Zip();
                                resultParms.push_back(Parameter(e1));
                                resultParms.push_back(Parameter(nowLesson->ExamPlace(), false));
                            }
                        }
                        Student* nowStudent = studentGroup.GetStudent(parms[1].number);
                        
                        sendAll(i, resultParms, false);
                        break;
                    }

                    case 0x0B : {//查询活动
                        Vector<Parameter> resultParms;
                        Student* nowStudent = studentGroup.GetStudent(parms[1].number);
                        Log("[活动列表查询] 学生 " + nowStudent->name + " 查询了活动列表。");
                        Vector<int> activities = nowStudent->events->activities;
                        //printf("activity phase");
                        for(int j = 0; j < activities.size(); j++) {
                            Activity* nowActivity = activityGroup.GetActivity(activities[j]);
                            //resultParms.push_back(Parameter(i));
                            resultParms.push_back(Parameter(nowActivity->name, false));
                            //resultParms.push_back(Parameter(lessonGroup.GetLesson[i]->Time(), false));
                            resultParms.push_back(Parameter(nowActivity->place, false));
                            Duration d = nowActivity->duration;
                            String time;
                            time = time + " " + ToString_Time(d.begin.hour) + ':' + ToString_Time(d.begin.minute) + '-' + ToString_Time(d.end.hour) + ':' + ToString_Time(d.end.minute) + ' ';
                            resultParms.push_back(Parameter(time, false));
                            resultParms.push_back(Parameter(activities[j]));
                            resultParms.push_back(Parameter(nowActivity->type));
                            int week = d.begin.week;
                            int day = d.begin.day;
                            resultParms.push_back(Parameter(week));
                            resultParms.push_back(Parameter(day));
                        }
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x0C : {//检索活动 update
                        Vector<Parameter> resultParms;
                        Student* nowStudent = studentGroup.GetStudent(parms[1].number);

                        Vector<String> modeString;
                        modeString.push_back("活动名称");
                        modeString.push_back("活动地点");
                        modeString.push_back("活动时间");
                        Log("[活动列表检索] 学生 " + nowStudent->name + " 检索了活动列表, 检索类型为 " + modeString[parms[2].number] + " 关键词为 " + parms[3].message);

                        int type = parms[2].number;
                        switch(type) {
                            case 0: {//活动名称
                                Vector<Activity*> result = nowStudent->events->FromActivityName(parms[3].message);
                                for(int j = 0; j < result.size(); j++) {
                                    resultParms.push_back(Parameter(result[j]->name, false));
                                    //resultParms.push_back(Parameter(lessonGroup.GetLesson[i]->Time(), false));
                                    resultParms.push_back(Parameter(result[j]->place, false));
                                    Duration d = result[j]->duration;
                                    String time;
                                    time = time + " " + ToString_Time(d.begin.hour) + ':' + ToString_Time(d.begin.minute) + '-' + ToString_Time(d.end.hour) + ':' + ToString_Time(d.end.minute) + ' ';
                                    resultParms.push_back(Parameter(time, false));
                                    resultParms.push_back(Parameter(result[j]->activityId));
                                    resultParms.push_back(Parameter(result[j]->type));
                                    int week = d.begin.week;
                                    int day = d.begin.day;
                                    resultParms.push_back(Parameter(week));
                                    resultParms.push_back(Parameter(day));
                                }
                                break;
                            }
                            case 1: {//活动地点
                                Vector<Activity*> result = nowStudent->events->FromActivityPlace(parms[3].message);
                                for(int j = 0; j < result.size(); j++) {
                                    resultParms.push_back(Parameter(result[j]->name, false));
                                    //resultParms.push_back(Parameter(lessonGroup.GetLesson[i]->Time(), false));
                                    resultParms.push_back(Parameter(result[j]->place, false));
                                    Duration d = result[j]->duration;
                                    String time;
                                    time = time + " " + ToString_Time(d.begin.hour) + ':' + ToString_Time(d.begin.minute) + '-' + ToString_Time(d.end.hour) + ':' + ToString_Time(d.end.minute) + ' ';
                                    resultParms.push_back(Parameter(time, false));
                                    resultParms.push_back(Parameter(result[j]->activityId));
                                    resultParms.push_back(Parameter(result[j]->type));
                                    int week = d.begin.week;
                                    int day = d.begin.day;
                                    resultParms.push_back(Parameter(week));
                                    resultParms.push_back(Parameter(day));
                                }

                                break;
                            }
                            case 2: {//活动时间
                                const char* time = parms[3].message.c_str();
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
                                Vector<int> result = nowStudent->events->activities;
                                for(int j = 0; j < result.size(); j++) {
                                    Activity* nowActivity = activityGroup.GetActivity(result[j]);
                                    if(nowActivity->duration.begin.HMLessEqual(begin) &&
                                        begin.HMLessEqual(activityGroup.GetActivity(j)->duration.end)) {
                                        resultParms.push_back(Parameter(nowActivity->name, false));
                                        //resultParms.push_back(Parameter(lessonGroup.GetLesson[i]->Time(), false));
                                        resultParms.push_back(Parameter(nowActivity->place, false));
                                        Duration d = nowActivity->duration;
                                        String time;
                                        time = time + " " + ToString_Time(d.begin.hour) + ':' + ToString_Time(d.begin.minute) + '-' + ToString_Time(d.end.hour) + ':' + ToString_Time(d.end.minute) + ' ';
                                        resultParms.push_back(Parameter(time, false));
                                        resultParms.push_back(Parameter(j));
                                        resultParms.push_back(Parameter(nowActivity->type));
                                        int week = d.begin.week;
                                        int day = d.begin.day;
                                        resultParms.push_back(Parameter(week));
                                        resultParms.push_back(Parameter(day));
                                    }
                                }

                                break;
                            }

                        }
                        sendAll(i, resultParms, false);
                        break;

                    }
                    case 0x0D : {//活动上传
                        Student* nowStudent = studentGroup.GetStudent(parms[5].number);

                        Log("[活动添加] 学生 " + nowStudent->name + " 添加了活动。");

                        Vector<Student*> nowStudents;
                        nowStudents.push_back(nowStudent);
                        int week = parms[4].number;
                        int day = parms[5].number;
                        const char* time = parms[6].message.c_str();
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
                            Timer(beginHour, beginMin, day, week),
                            Timer(endHour,endMin, day, week)
                        );
                        bool check = true;
                        Vector<Duration> vv;
                        vv.push_back(tmpDuration);
                        for(int i = 0; i < nowStudents.size(); i++) {
                            check &= nowStudents[i]->events->VerifyDuration(vv);
                        }
                        int type = parms[3].number == 1;//5 id
                        for(int j = 1; j < parms[3].number; j++) {
                            nowStudents.push_back(studentGroup.GetStudent(parms[j+7].number));
                            printf("%d\n", parms[j+7].number);
                        }
                        Vector<Parameter> resultParms;
                        if(check) {
                            Activity* nowActivity = new Activity(parms[1].message, parms[2].message, type, tmpDuration, nowStudents);
                            int activityID = activityGroup.AddActivities(nowActivity);
                            nowActivity->activityId = activityID;
                            studentGroup.GetStudent(parms[7].number)->events->AddActivity(activityID);
                            resultParms.push_back(Parameter(true));
                        } else {
                            resultParms.push_back(Parameter(false));
                        }
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x0E : {// 活动文件上传 update
                        Student* nowStudent = studentGroup.GetStudent(parms[1].number);
                        int id = parms[2].number;
                        Activity* nowActivity = activityGroup.GetActivity(id);
                        String savePath = "Activity/" + ToString(id) + "/" + parms[3].message;
                        String tmpHash = sha256(parms[4].message);
                        File* file = new File(savePath, parms[3].message, tmpHash);
                        nowActivity->AddFile(file);
                        WriteFile(savePath, parms[4].message);

                        Log("[活动文件上传] 学生 " + nowStudent->name + " 给活动 " + nowActivity->name + " 上传了文件。");

                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(String("ack"), false));
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x0F : { // 课程文件上传 update
                        int id = parms[3].number;
                        Lesson* nowLesson = lessonGroup.GetLesson(id);
                        String savePath = "Lesson/" + ToString(id) +"/" + parms[4].message;
                        String tmpHash = sha256(parms[5].message);
                        File* file = new File(savePath, parms[4].message, tmpHash);
                        nowLesson->AddFile(file);
                        WriteFile(savePath, parms[5].message);

                        if(parms[1].number == 0) {
                            Student* nowStudent = studentGroup.GetStudent(parms[2].number);
                            Log("[课程文件上传] 学生 " + nowStudent->name + " 给课程 " + nowLesson->Name() + " 上传了文件。");
                        } else {
                            Teacher* nowTeacher = teacherGroup.GetTeacher(parms[2].number);
                            Log("[课程文件上传] 教师 " + nowTeacher->name + " 给课程 " + nowLesson->Name() + " 上传了文件。");
                        }

                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(String("ack"), false));
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x10 : { // 活动文件下载

                    }
                    case 0x11 : { // 课程文件下载 log undone
                        printf("begin download\n");
                        Vector<Parameter> resultParms;
                        int id = parms[3].number;
                        Lesson* nowLesson = lessonGroup.GetLesson(id);
                        if(parms[1].number == 0) {
                            Student* nowStudent = studentGroup.GetStudent(parms[2].number);
                            Log("[课程文件下载] 学生 " + nowStudent->name + " 下载了 " + nowLesson->Name() + " 的课程文件。");
                        } else {
                            Teacher* nowTeacher = teacherGroup.GetTeacher(parms[2].number);
                            Log("[课程文件下载] 老师 " + nowTeacher->name + " 下载了 " + nowLesson->Name() + " 的课程文件。");
                        }
                        String savePath = "Lesson/" + ToString(id) + "/"  + parms[4].message;
                        FILE* file;
                        if((file = fopen(savePath.c_str(), "rb")) == NULL) {
                            printf("file not found\n");
                            break;
                        } else {
                            FILE* transfile = fopen("tmpfile", "wb");
                            decodeSolver.Decode(file, transfile, 0, 0);
                            transfile = fopen("tmpfile", "rb");
                            String download;
                            while(!feof(transfile)) {
                                unsigned char Cx;
                                if(fread(&Cx, 1, 1, transfile) > 0) {
                                    download.push_back(Cx);
                                } else break;
                            }
                            fclose(transfile);
                            resultParms.push_back(Parameter(download, true));
                            sendAll(i, resultParms, true);
                            break;
                        }
                    }
                    
                    case 0x12 : {//检索当前有无触发信息
                        //printf("Send trigger lists");
                        Vector<Parameter> resultParms;
                        Student* nowStudent = studentGroup.GetStudent(parms[1].number);
                        resultParms.push_back(Parameter(nowStudent->triggerMessage.size()));
                        for(int j = 0; j < nowStudent->triggerMessage.size(); j++) {
                            resultParms.push_back(Parameter(nowStudent->triggerMessage[j], false));
                        }
                        nowStudent->ClearTrigger();
                        sendAll(i, resultParms, false);
                        break;
                    }
                    case 0x13 : {// 待办列表
                        Vector<Parameter> resultParms;
                        Student* nowStudent = studentGroup.GetStudent(parms[1].number);

                        Log("[闹钟查询] 学生 " + nowStudent->name + " 查询了闹钟列表。");

                        resultParms.push_back(Parameter(nowStudent->alarms.size()));
                        for(int j = 0; j < nowStudent->alarms.size(); j++) {
                            Alarm* nowAlarm = alarmGroup.FromId(nowStudent->alarms[j]);        
                            resultParms.push_back(Parameter(nowAlarm->t.Zip()));
                            resultParms.push_back(Parameter(nowAlarm->frequency));
                            resultParms.push_back(Parameter(nowAlarm->desc, false));
                            resultParms.push_back(Parameter(nowAlarm->place, false));
                            resultParms.push_back(Parameter(nowAlarm->id));
                            resultParms.push_back(Parameter(nowAlarm->enabled));
                        }
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0x14 : { // 修改闹钟 update
                        Student* nowStudent = studentGroup.GetStudent(parms[1].number);

                        Log("[闹钟修改] 学生 " + nowStudent->name + " 修改了闹钟。");
                        Alarm* nowAlarm = alarmGroup.FromId(parms[2].number);
                        timeHeap.Delete(nowAlarm->id);
                        nowAlarm->updateMsg(UnzipTimer(parms[3].number), parms[4].number, parms[5].message, parms[6].message, parms[7].number);
                        if(nowAlarm->enabled = true) {
                            timeHeap.Push(ClockTimer(nowAlarm->GetNextTime(timeTracker.NowTimer()), nowAlarm->id));
                        }
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(String("ack"), false));
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0x15 : { // 添加闹钟
                        Alarm* nowAlarm = new Alarm(
                            UnzipTimer(parms[1].number),
                            parms[2].number,
                            parms[3].message,
                            parms[4].message,
                            parms[5].number,
                            GenerateAlarmId(),
                            parms[6].number);
                        alarmGroup.AddAlarm(nowAlarm);
                        Student* nowStudent = studentGroup.GetStudent(parms[5].number);
                        nowStudent->AddAlarm(nowAlarm->id);

                        Log("[闹钟添加] 学生 " + nowStudent->name + " 添加了闹钟。");

                        timeHeap.Push(ClockTimer(nowAlarm->GetNextTime(timeTracker.NowTimer()), nowAlarm->id));
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(String("ack"), false));
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0X16 : { // 删除闹钟
                        printf("Alarm deleted!\n");
                        Alarm* nowAlarm = alarmGroup.FromId(parms[1].number);
                        timeHeap.Delete(nowAlarm->id);
                        alarmGroup.DeleteAlarm(nowAlarm);
                        Student* nowStudent = studentGroup.GetStudent(nowAlarm->belongId);
                        nowStudent->DeleteAlarm(parms[1].number);

                        Log("[闹钟删除] 学生 " + nowStudent->name + " 删除了闹钟。");

                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(String("ack"), false));
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0x17 : { // infoquery
                        printf("userinfo query!\n");
                        Student* nowStudent = studentGroup.GetStudent(parms[1].number);
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(nowStudent->name, false));
                        resultParms.push_back(Parameter(nowStudent->studentNumber, false));
                        resultParms.push_back(Parameter(parms[1].number));
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0x18 : { // passwordupd
                        printf("password query!\n");
                        if(parms[1].number == 0) {
                            Student* nowStudent = studentGroup.GetStudent(parms[2].number);
                            Log("[密码更改] 学生 " + nowStudent->name + " 更改了密码。");
                            nowStudent->password = parms[3].message;
                        } else {
                            Teacher* nowTeacher = teacherGroup.GetTeacher(parms[2].number);
                            Log("[密码更改] 老师 " + nowTeacher->name + " 更改了密码。");
                            nowTeacher->password = parms[3].message;
                        }
                        
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(String("ack"), false));
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0x19 : {//change class info
                        /*
                         * @params1: classId
                         * @params2: name
                         * @params3: teacher
                         * @params4: place
                         * @params5: beginTime
                         * @params6: endTime
                         * @params7: qq
                         * @params8: examBegin
                         * @params9: examEnd
                         * @params10: examPlace
                         * todo change time how to deal with it
                         */
                        printf("exam changed\n");
                        
                        Lesson* nowClass = lessonGroup.GetLesson(parms[2].number);
                        Timer classBeginTime = UnzipTimer(parms[6].number);
                        Timer classEndTime = UnzipTimer(parms[7].number);
                        Vector<Duration> classTime;
                        classTime.push_back(Duration(classBeginTime, classEndTime));
                        Timer beginTime = UnzipTimer(parms[9].number);
                        Timer endTime = UnzipTimer(parms[10].number);
                        Duration dura(beginTime, endTime);
                        nowClass->updateInfo(parms[3].message, parms[4].message, parms[5].message, classTime, parms[8].message, dura, parms[11].message);
                        
                        Teacher* nowTeacher = teacherGroup.GetTeacher(parms[1].number);
                        Log("[修改课程信息] 教师 " + nowTeacher->name + " 修改了课程 " + nowClass->Name() + " 的课程信息。");
                        
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(String("ack"), false));
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0x1A : { // teacherinfoquery
                        Teacher* nowTeacher = teacherGroup.GetTeacher(parms[1].number);
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(nowTeacher->name, false));
                        resultParms.push_back(Parameter(nowTeacher->teacherNumber, false));
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0X1B : { // student query
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(studentGroup.students.size()));
                        for(int j = 0; j < studentGroup.students.size(); j++) {
                            resultParms.push_back(Parameter(studentGroup.students[j]->name, false));
                            resultParms.push_back(Parameter(studentGroup.students[j]->studentNumber, false));
                            resultParms.push_back(Parameter(j));
                        }
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0x1C : {
                        Vector<Parameter> resultParms;
                        Teacher* nowTeacher = teacherGroup.GetTeacher(parms[2].number);
                        Vector<Duration> tmpDurations;
                        Timer bgTimer = Timer(beg[parms[8].number].hour, beg[parms[8].number].minute, parms[7].number, parms[5].number);
                        Timer edTimer = Timer(beg[parms[9].number].hour, beg[parms[9].number].minute, parms[7].number, parms[6].number);
                        tmpDurations.push_back(Duration(bgTimer, edTimer));
                        Vector<Student*> students;
                        for(int j = 0, k = 11; j < parms[10].number; j++, k++) {
                            students.push_back(studentGroup.GetStudent(parms[k].number));
                        }
                        Lesson* tmpLesson =new Lesson(parms[1].message, nowTeacher->name, parms[3].message, parms[4].message, tmpDurations, students);
                        int lessonId = lessonGroup.AddLesson(tmpLesson);
                        tmpLesson->lessonId = lessonId;
                        for(int j = 0; j < students.size(); j++) {
                            students[j]->events->AddLesson(lessonId);
                            students[j]->TriggerMessage("   新增课程： 您有一门新增课程 " + parms[3].message);
                        }
                        nowTeacher->AddLesson(lessonId);
                        resultParms.push_back(Parameter("Ack", false));
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0X1D : { //teacher home work query
                        Teacher* nowTeacher = teacherGroup.GetTeacher(parms[1].number);
                        Lesson* nowLesson = lessonGroup.GetLesson(parms[2].number);
                        Vector<Parameter> resultParms;
                        Vector<Homework*> homework = nowLesson->homeworks;
                        Log("[查询作业集合] 教师 " + nowTeacher->name + " 查询了课程 " + lessonGroup.GetLesson(nowLesson->lessonId)->Name() + " 的作业集合。");
                        resultParms.push_back(Parameter(homework.size()));
                        for (int i = 0; i < homework.size(); i++) {
                            resultParms.push_back(Parameter(homework[i]->desc, false));
                        }
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0x1E : { // start timer
                        timeTracker.restart();
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(String("ack"), false));
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0x1F : { // stop timer
                        timeTracker.stop();
                        Vector<Parameter> resultParms;
                        resultParms.push_back(Parameter(String("ack"), false));
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0x20 : { //query 地图信息
                        printf("Map Query!");
                        Vector<Parameter> resultParms;
                        Vector<int> result;
                        int mode = parms[1].number;
                        for(int i = 1; i < graph.num; i += 2) if(graph.e[i].Ori() < graph.crossEdgeId) {
                            switch (mode) {
                                case 1 :
                                    result.push_back(graph.e[i].Dis());
                                    break;
                                case 2 :
                                    result.push_back(graph.e[i].Dis() / (humanSpeed * graph.e[i].Ratio()));
                                    break;
                                case 3 :
                                    result.push_back(graph.e[i].Dis() / ((graph.e[i].Type() ? bicycleSpeed : humanSpeed) * graph.e[i].Ratio()));
                                    break;
                            }
                        }
                        result.push_back(graph.crossTime);
                        for(int i = 1; i < graph.num; i += 2) if(graph.e[i].Ori() > graph.crossEdgeId) {
                            switch (mode) {
                                case 1 :
                                    result.push_back(graph.e[i].Dis());
                                    break;
                                case 2 :
                                    result.push_back(graph.e[i].Dis() / (humanSpeed * graph.e[i].Ratio()));
                                    break;
                                case 3 :
                                    result.push_back(graph.e[i].Dis() / ((graph.e[i].Type() ? bicycleSpeed : humanSpeed) * graph.e[i].Ratio()));
                                    break;
                            }
                        }
                        printf("size: %d\n", result.size());
                        resultParms.push_back(Parameter(result.size()));
                        for(int j = 0; j < result.size(); j++) {
                            resultParms.push_back(Parameter(result[j]));
                        }
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0x21 : { // query place
                        Vector<Parameter> resultParms;
                        Vector<Lesson*> lessons = studentGroup.GetStudent(parms[1].number)->events->FromLessonName(parms[2].message);
                        if(lessons.size()) {
                            resultParms.push_back(Parameter(graph.GetPointId(lessons[0]->Place())));
                        } else {
                            resultParms.push_back(Parameter(6666));
                        }
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0x22 : { // query place
                        Vector<Parameter> resultParms;
                        Vector<Lesson_Student*> lessons = studentGroup.GetStudent(parms[1].number)->events->lessons;
                        Timer t1 = timeTracker.NowTimer();
                        Timer t2 = UnzipTimer(parms[2].number);
                        Timer t3 = Timer(t2.hour, t2.minute, t1.day, t1.week);
                        Timer lessTimer;
                        Lesson* resultLesson = NULL;
                        for(int j = 0; j < lessons.size(); j++) {
                            Lesson* nowLesson = lessonGroup.GetLesson(lessons[j]->lessonId);
                            Vector<Duration> nowDurations = nowLesson->ClassDurations();
                            bool find = false;
                            for(int k = 0; k < nowDurations.size(); k++) {
                                if(nowDurations[k].begin.week <= t3.week && t3.week <= nowDurations[k].end.week &&
                                    nowDurations[k].begin.day == t3.day && t3.HMLess(nowDurations[k].begin)) {
                                    if(!resultLesson || nowDurations[k].begin.HMLess(lessTimer)) {
                                        resultLesson = nowLesson;
                                        lessTimer = nowDurations[k].begin;
                                        find = true;
                                    }
                                }
                                if(find) break;
                            }
                        }
                        if(resultLesson) {
                            resultParms.push_back(Parameter(graph.GetPointId(resultLesson->Place())));
                        } else {
                            resultParms.push_back(Parameter(6666));
                        }
                        sendAll(i, resultParms, true);
                        break;
                    }
                    case 0x23 : {
                        Vector<Parameter> resultParms;
                        EventGroup* events = studentGroup.GetStudent(parms[1].number)->events;
                        String resultString;
                        for(int j = 0; j < events->lessons.size(); j++) {
                            Lesson* nowLesson =  lessonGroup.GetLesson(events->lessons[j]->lessonId);
                            for(int k = 0; k < nowLesson->ClassDurations().size(); k++) {
                                Duration td = nowLesson->ClassDurations()[k];
                                if(td.begin.day == parms[2].number && td.begin.week <= timeTracker.NowTimer().week && timeTracker.NowTimer().week <= td.end.week) {
                                    if(td.begin.HMLessEqual(beg[parms[3].number]) && beg[parms[3].number].HMLessEqual(td.end)) {
                                        resultString = nowLesson->Name() + "\n" + ToString(td.begin.week) + "-" + ToString(td.end.week) + "周";
                                    }
                                }
                            }
                        }
                        for(int j = 0; j < events->activities.size(); j++) {
                            Activity* nowActivity = activityGroup.GetActivity(events->activities[j]);
                            Duration td = nowActivity->duration;
                            if(td.begin.day == parms[2].number && td.begin.week <= timeTracker.NowTimer().week && timeTracker.NowTimer().week <= td.end.week) {
                                if(td.begin.HMLessEqual(beg[parms[3].number]) && beg[parms[3].number].HMLessEqual(td.end)) {
                                    resultString = nowActivity->name + "\n" + ToString(td.begin.week) + "-" + ToString(td.end.week) + "周";
                                }
                            }
                        }
                        resultParms.push_back(Parameter(resultString, false));
                        sendAll(i, resultParms, true);
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