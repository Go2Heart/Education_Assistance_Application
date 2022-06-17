#include "connect.h"
#include "global.h"

int UnZip(std::string x) {
    int y = 0;
    for(int i = 0; i < x.length(); i++) y = (y << 8) + (unsigned char)x[i];
    return y;
}

TcpConnector::TcpConnector(QVector<Parameter*> message) : m(message)
{
    socket = new QTcpSocket(this);
#ifdef __WIN32__
    //socket->connectToHost("82.157.164.204", 43434);
    socket->connectToHost("123.56.124.140", 8888);
#endif
#ifdef  __APPLE__
    socket->connectToHost("82.157.164.204", 43434);
#endif
    connect(socket, SIGNAL(readyRead()), SLOT(readData()));
    connect(socket, SIGNAL(connected()), SLOT(connected()));
}

void TcpConnector::connected() {
    //qDebug()<<"send start";
    int sendlen = 0;
    for(int i = 0; i < m.size(); i++) {
        sendlen = sendlen + m[i]->s.length();
    }
    //qDebug()<<"sendlen:"<<sendlen;
    char len[4];
    int tmpint = sendlen;
    for(int i = 3; i >= 0; i--) len[i] = tmpint % 256, tmpint >>= 8;
    socket->write(len, 4);
    for(int i = 0; i < m.size(); i++) {
        char buf[1024];
        for(int j = 0; j < (int)m[i]->s.length(); j += 1024) {
            int nowlen = std::min(1024, (int)m[i]->s.length() - j);
            for(int k = 0; k < nowlen; k++) buf[k] = m[i]->s[j + k];
            socket->write(buf, nowlen);
        }
    }
    socket->flush();
    //qDebug()<<"send end";
}

void TcpConnector::readData() {
    //qDebug()<<"get answer!";
    QByteArray nowbyte = socket->readAll();
    //qDebug()<<nowbyte.length();
    int beginIdx = 0;
    if(!readSize) {
        for(int i = 0; i < 4; i++) receiveLen = (receiveLen << 8) + (unsigned char)nowbyte[i];
        readSize = true;
        beginIdx += 4;
        //qDebug()<<receiveLen;
    }
    for(int i = beginIdx; i < nowbyte.length(); i++) readBuf.push_back(nowbyte[i]);
    receiveLen -= nowbyte.length() - beginIdx;

    if(!receiveLen) {
        //qDebug()<<"finish recv";
        QVector<Parameter*> parms;
        int idx = 0;
        //qDebug() << "readBuf len:"<<readBuf.length();
        while(idx < readBuf.length()) {
            std::string parm;
            int len = 0;
            int type = readBuf[idx];
            idx++;
            for(int j = idx; j <= idx + 3; j++)
                len = (len << 8) + (unsigned char)readBuf[j];
            idx += 4;
            for(int j = idx; j < idx + len; j++) parm.push_back(readBuf[j]);
            idx += len;
            switch (type) {
                case Parameter::STRING:
                    parms.push_back(new Parameter(QString::fromUtf8(parm.data(), parm.length())));
                    break;
                case Parameter::FILE:
                    parms.push_back(new Parameter(parm));
                    break;
                case Parameter::INT:
                    parms.push_back(new Parameter(UnZip(parm)));
                    break;
            }
        }
        emit receive(QVariant::fromValue(parms));
        socket->close();
        socket->deleteLater();
    }
}

HomeworkPost::HomeworkPost(int id, QString desc) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x01));
    paras.push_back(new Parameter(teacherId));
    paras.push_back(new Parameter(id));
    paras.push_back(new Parameter(desc));
    connector = new TcpConnector(paras);  
}

HomeworkUpload::HomeworkUpload(int studentId, int classId, int homeworkId, int count, QVector<QString> fileNames,
                               QVector<std::string> fileData) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x02));
    paras.push_back(new Parameter(studentId));
    paras.push_back(new Parameter(classId));
    paras.push_back(new Parameter(homeworkId));
    paras.push_back(new Parameter(count));
    for(int i = 0; i < fileNames.size(); i++) {
        paras.push_back(new Parameter(fileNames[i]));
        paras.push_back(new Parameter(fileData[i]));
    }
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        emit receive(parms[0]->number);
    });
}

HomeworkQuery::HomeworkQuery(int studentId, int classId) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x03));
    paras.push_back(new Parameter(studentId));
    paras.push_back(new Parameter(classId));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        QVector<HomeworkResult*> v;
        int size = parms[0]->number;
        for(int i = 0, j = 1; i < size; i++, j += 3) {
            v.push_back(new HomeworkResult( parms[j]->number, parms[j + 1]->number, parms[j + 2]->qsMessage));
        }
        emit receive(QVariant::fromValue(v));
    });
}

HomeworkSearch::HomeworkSearch(int studentId, int classId, QString key) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x04));
    paras.push_back(new Parameter(studentId));
    paras.push_back(new Parameter(classId));
    paras.push_back(new Parameter(key));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        QVector<HomeworkResult*> v;
        for(int i = 0; i < parms.size(); i += 3) {
            v.push_back(new HomeworkResult( parms[i]->number, parms[i + 1]->number, parms[i + 2]->qsMessage));
        }
        emit receive(QVariant::fromValue(v));
    });
}

DisQuery::DisQuery(QVector<int> v, int mode) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x05));
    paras.push_back(new Parameter(studentId));
    paras.push_back(new Parameter(v.size()));
    for(int i = 0; i < v.size(); i++) paras.push_back(new Parameter(v[i]));
    paras.push_back(new Parameter(mode));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        Timer nowTimer = UnzipTimer(parms[0]->number);
        int size = parms[1]->number;
        QVector<Result> v;
        for(int i = 1, j = 2; i <= size; i++, j += 3)
            v.push_back(Result(parms[j]->number, parms[j + 1]->number, parms[j + 2]->number));
        emit receive(QVariant::fromValue(ResPackage(nowTimer, v)));
    });
}

TimeQuery::TimeQuery() {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x06));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
       QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
       emit receive(parms[0]->number);
    });
}

TimeSpdChg::TimeSpdChg(int x) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x07));
    paras.push_back(new Parameter(type));
    if(type == 0) paras.push_back(new Parameter(studentId));
    else paras.push_back(new Parameter(teacherId));
    paras.push_back(new Parameter(x));
    connector = new TcpConnector(paras);
}

LoginQuery::LoginQuery(int type, QString name, QString password) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x08));
    paras.push_back(new Parameter(type));
    paras.push_back(new Parameter(name));
    paras.push_back(new Parameter(password));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
       QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
       emit Id(parms[0]->number);
    });
}

ClassQuery::ClassQuery() {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x09));
    paras.push_back(new Parameter(type));
    if(type == 0) paras.push_back(new Parameter(studentId));
    else paras.push_back(new Parameter(teacherId));
    //qDebug() << id;
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        QVector<ClassResult*> v;
        /*for(int i = 0; i < parms.size(); i += 6) {
           v.push_back(new ClassResult( parms[i]->qsMessage, parms[i + 1]->qsMessage, parms[i + 2]->qsMessage, parms[i + 3]->qsMessage, parms[i + 4]->qsMessage, parms[i + 5]->qsMessage));
        }*/
        int i = 0;
        while(i < parms.size()) {
            QString name = parms[i]->qsMessage;
            QString teacher = parms[i + 1]->qsMessage;
            QString place = parms[i + 2]->qsMessage;
            QString time = parms[i + 3]->qsMessage;
            QString QQ = parms[i + 4]->qsMessage;
            int id = parms[i + 5]->number;
            QVector<QString> files;
            int fileNum = parms[i + 6]->number;
            qDebug() <<"parms[i + 6]->qsMessage: " <<parms[i + 6]->qsMessage << "parms[i + 6]->number: "<< parms[i + 6]->number;
            qDebug() << name << teacher << place << time << QQ << id << fileNum;
            for(int j = 0; j < fileNum; j++) {
                files.push_back(parms[i + 7 + j]->qsMessage);
                qDebug() << "fileNum:" << fileNum;
                qDebug() << "filename:" << parms[i + 7 + j]->qsMessage;
            }
            Timer examBegin = UnzipTimer(parms[i + 7 + fileNum]->number);
            Timer examEnd = UnzipTimer(parms[i + 8 + fileNum]->number);
            QString examPlace = parms[i + 9 + fileNum]->qsMessage;
            qDebug() << "--" << examPlace;
            i += 10 + fileNum;
            v.push_back(new ClassResult(name, teacher, place, time, QQ, id, files, examBegin, examEnd, examPlace));
        }
        //qDebug()<<"ClassQuery";
        emit receive(QVariant::fromValue(v));
    });
}

ClassSearch::ClassSearch(QString key, int searchtype) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x0A));
    paras.push_back(new Parameter(type));
    if(type == 0) paras.push_back(new Parameter(studentId));
    else paras.push_back(new Parameter(teacherId));
    paras.push_back(new Parameter(key));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        QVector<ClassResult*> v;
        /*for(int i = 0; i < parms.size(); i += 6) {
            v.push_back(new ClassResult( parms[i]->qsMessage, parms[i + 1]->qsMessage, parms[i + 2]->qsMessage, parms[i + 3]->qsMessage, parms[i + 4]->qsMessage, parms[i + 5]->qsMessage));
        }*/
        int i = 0;
        while(i < parms.size()) {
            QString name = parms[i]->qsMessage;
            QString teacher = parms[i + 1]->qsMessage;
            QString place = parms[i + 3]->qsMessage;
            QString time = parms[i + 2]->qsMessage;
            QString QQ = parms[i + 4]->qsMessage;
            int id = parms[i + 5]->number;
            QVector<QString> files;
            int fileNum = parms[i + 6]->number;
            qDebug() <<"parms[i + 6]->qsMessage: " <<parms[i + 6]->qsMessage << "parms[i + 6]->number: "<< parms[i + 6]->number;
            qDebug() << name << teacher << place << time << QQ << id << fileNum;
            for(int j = 0; j < fileNum; j++) {
                files.push_back(parms[i + 7 + j]->qsMessage);
                qDebug() << "fileNum:" << fileNum;
                qDebug() << "filename:" << parms[i + 7 + j]->qsMessage;
            }
            Timer examBegin = UnzipTimer(parms[i + 7 + fileNum]->number);
            Timer examEnd = UnzipTimer(parms[i + 8 + fileNum]->number);
            QString examPlace = parms[i + 9 + fileNum]->qsMessage;
            i += 10 + fileNum;
            v.push_back(new ClassResult(name, teacher, place, time, QQ, id, files, examBegin, examEnd, examPlace));
        }
        //qDebug()<<"ClassQuery";
        emit receive(QVariant::fromValue(v));
    });
}


ActivityQuery::ActivityQuery(int id) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x0B));
    paras.push_back(new Parameter(id));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        QVector<ActivityResult*> v;
        int cnt=0;
        for(int i = 0; i < parms.size(); i += 7) {
            cnt++;
            v.push_back(new ActivityResult( parms[i]->qsMessage, parms[i + 1]->qsMessage, parms[i + 2]->qsMessage, parms[i + 3]->number, parms[i + 4]->number, parms[i + 5]->number, parms[i + 6]->number));
        }
        qDebug() <<  "receive activity, size:" <<cnt;
        emit receive(QVariant::fromValue(v));
    });
}

ActivitySearch::ActivitySearch(QString name, int type) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x0C));
    paras.push_back(new Parameter(studentId));
    paras.push_back(new Parameter(type));
    paras.push_back(new Parameter(name));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        QVector<ActivityResult*> v;
        for(int i = 0; i < parms.size(); i += 7) {
            v.push_back(new ActivityResult( parms[i]->qsMessage, parms[i + 1]->qsMessage, parms[i + 2]->qsMessage, parms[i + 3]->number, parms[i + 4]->number, parms[i + 5]->number, parms[i + 6]->number));
        }
        emit receive(QVariant::fromValue(v));
    });

}

ActivityUpload::ActivityUpload(QVector<QString> v, int id) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x0D));
    paras.push_back(new Parameter(v[0])); //place
    paras.push_back(new Parameter(v[1])); //name
    paras.push_back(new Parameter(v[2].toInt())); //type
    qDebug() << "v[2].toInt(): " << v[2].toInt();
    paras.push_back(new Parameter(v[3].toInt())); //week
    paras.push_back(new Parameter(v[4].toInt())); //day
    paras.push_back(new Parameter(v[5])); //time

    qDebug() << "activity time: " << v[3] << " " << v[4] << " - " << v[5];
    paras.push_back(new Parameter(id)); //content
    for(int i = 1; i < v[2].toInt(); i++) {
        paras.push_back(new Parameter(v[6 + i].toInt()));
        qDebug() << "v[6 + i].toInt(): " << v[6 + i].toInt();
    }
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        emit receive(parms[0]->number);
    });
}

FileUpload::FileUpload(QString id, QString descripter,std::string info, int studentId, int mode) {
    QVector<Parameter*> paras;
    if(mode == 0) {
        paras.push_back(new Parameter(0x0E));
        paras.push_back(new Parameter(studentId));
        if (id == "") paras.push_back(new Parameter(0));
        else paras.push_back(new Parameter(id.toInt()));
        paras.push_back(new Parameter(descripter));
        paras.push_back(new Parameter(info));
    } else {
        paras.push_back(new Parameter(0x0F));
        paras.push_back(new Parameter(type));
        if(type == 0) paras.push_back(new Parameter(studentId));
        else paras.push_back(new Parameter(teacherId));
        if (id == "") paras.push_back(new Parameter(0));
        else paras.push_back(new Parameter(id.toInt()));
        paras.push_back(new Parameter(descripter));
        paras.push_back(new Parameter(info));
    }
    qDebug() << "file upload student id: " << studentId;
    qDebug() << "id"<<  id<<"descripter" <<descripter;
    connector = new TcpConnector(paras);
}

FileDownload::FileDownload(QString id, QString descripter, int mode) {
    QVector<Parameter*> paras;
    if (mode == 0)paras.push_back(new Parameter(0x10));
    else paras.push_back(new Parameter(0x11));
    paras.push_back(new Parameter(type));
    if(type == 0) paras.push_back(new Parameter(studentId));
    else paras.push_back(new Parameter(teacherId));
    if (id == "")
        paras.push_back(new Parameter(0));
    else
        paras.push_back(new Parameter(id.toInt()));
    paras.push_back(new Parameter(descripter));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        FileResult* v = new FileResult(parms[0]->sMessage);
        emit receive(QVariant::fromValue(v));
    });
}

TriggersQuery::TriggersQuery() {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x12));
    paras.push_back(new Parameter(studentId));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        QVector<QString> ss;
        int size = parms[0]->number;
        for(int i = 0; i < size; i++) ss.push_back(parms[i + 1]->qsMessage);
        emit receive(QVariant::fromValue(ss));
    });
}

AlarmsQuery::AlarmsQuery() {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x13));
    paras.push_back(new Parameter(studentId));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        QVector<Alarm> v;
        int size = parms[0]->number;
        for(int i = 1, j = 1; i <= size; i++, j += 6)
            v.push_back(Alarm(UnzipTimer(parms[j]->number),
                parms[j + 1]->number,
                parms[j + 2]->qsMessage,
                parms[j + 3]->qsMessage,
                parms[j + 4]->number,
                parms[j + 5]->number));
        emit receive(QVariant::fromValue(v));
    });
}

AlarmModify::AlarmModify(int id, int timer, int frequency, QString desc, QString place, int enabled) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x14));
    paras.push_back(new Parameter(studentId));
    paras.push_back(new Parameter(id));
    paras.push_back(new Parameter(timer));
    paras.push_back(new Parameter(frequency));
    paras.push_back(new Parameter(desc));
    paras.push_back(new Parameter(place));
    paras.push_back(new Parameter(enabled));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        emit receive();
    });
}

AlarmAdd::AlarmAdd(int timer, int frequency, QString desc, QString place, int enabled) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x15));
    paras.push_back(new Parameter(timer));
    paras.push_back(new Parameter(frequency));
    paras.push_back(new Parameter(desc));
    paras.push_back(new Parameter(place));
    paras.push_back(new Parameter(studentId));
    paras.push_back(new Parameter(enabled));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        qDebug() << "socket query receive data!";
        emit receive();
    });
}

AlarmDel::AlarmDel(int id) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x16));
    paras.push_back(new Parameter(id));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        emit receive();
    });
}

StudentInfoQuery::StudentInfoQuery() {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x17));
    paras.push_back(new Parameter(studentId));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        emit receive(QVariant::fromValue(Student(parms[0]->qsMessage, parms[1]->qsMessage, parms[2]->number)));
    });
}

PasswordUpd::PasswordUpd(QString passwd) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x18));
    paras.push_back(new Parameter(type));
    if(type == STUDENT)
        paras.push_back(new Parameter(studentId));
    else
        paras.push_back(new Parameter(teacherId));
    paras.push_back(new Parameter(passwd));
    connector = new TcpConnector(paras);
}

ClassChange::ClassChange(int id, QString name, QString teacher, QString place, int classBegin, int classEnd,
                         QString QQ, int examBegin, int examEnd, QString examPlace) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x19));
    paras.push_back(new Parameter(teacherId));
    paras.push_back(new Parameter(id));
    paras.push_back(new Parameter(name));
    paras.push_back(new Parameter(teacher));
    paras.push_back(new Parameter(place));
    paras.push_back(new Parameter(classBegin));
    paras.push_back(new Parameter(classEnd));
    paras.push_back(new Parameter(QQ));
    paras.push_back(new Parameter(examBegin));
    paras.push_back(new Parameter(examEnd));
    paras.push_back(new Parameter(examPlace));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
       qDebug() << "socket query receive data!";
        emit receive();
    });
}

TeacherInfoQuery::TeacherInfoQuery() {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x1A));
    paras.push_back(new Parameter(teacherId));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        emit receive(QVariant::fromValue(Teacher(parms[0]->qsMessage, parms[1]->qsMessage)));
    });
}

StudentGroupQuery::StudentGroupQuery() {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x1B));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        QVector<Student> v;
        int size = parms[0]->number;
        for(int i = 0, j = 1; i < size; i++, j += 3) {
            v.push_back(Student(parms[j]->qsMessage, parms[j + 1]->qsMessage, parms[j + 2]->number));
        }
        emit receive(QVariant::fromValue(v));
    });
}

ClassAddQuery::ClassAddQuery(QString place, QString name, QString QQ, int beginwk, int endwk, int day, int bgNum, int edNum, QVector<Student> students) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x1C));
    paras.push_back(new Parameter(place));
    paras.push_back(new Parameter(teacherId));
    paras.push_back(new Parameter(name));
    paras.push_back(new Parameter(QQ));
    paras.push_back(new Parameter(beginwk));
    paras.push_back(new Parameter(endwk));
    paras.push_back(new Parameter(day));
    paras.push_back(new Parameter(bgNum));
    paras.push_back(new Parameter(edNum));
    paras.push_back(new Parameter(students.size()));
    for(int i = 0; i < students.size(); i++) {
        paras.push_back(new Parameter(students[i].id));
    }
    connector = new TcpConnector(paras);
}

TeacherHomeworkQuery::TeacherHomeworkQuery(int id) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x1D));
    paras.push_back(new Parameter(teacherId));
    paras.push_back(new Parameter(id));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        QVector<TeacherHomework*> v;
        int size = parms[0]->number;
        for(int i = 0; i < size; i++) {
            v.push_back(new TeacherHomework(parms[i+1]->qsMessage));
            qDebug() << "received" << parms[i+1]->qsMessage;
        }
        emit receive(QVariant::fromValue(v));
    });
}

StartTimer::StartTimer() {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x1E));
    connector = new TcpConnector(paras);
}

StopTimer::StopTimer() {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x1F));
    connector = new TcpConnector(paras);
}

MapQuery::MapQuery(int mode) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x20));
    paras.push_back(new Parameter(mode));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        int size = parms[0]->number;
        QVector<int> v;
        for(int i = 1; i <= size; i++) v.push_back(parms[i]->number);
        emit receive(QVariant::fromValue(v));
    });
}

ClassPointQuery::ClassPointQuery(QString name) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x21));
    paras.push_back(new Parameter(studentId));
    paras.push_back(new Parameter(name));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        emit receive(parms[0]->number);
    });
}

TimePointQuery::TimePointQuery(Timer t) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x22));
    paras.push_back(new Parameter(studentId));
    paras.push_back(new Parameter(t.Zip()));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        emit receive(parms[0]->number);
    });
}

TableQuery::TableQuery(int day, int num) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(0x23));
    paras.push_back(new Parameter(studentId));
    paras.push_back(new Parameter(day));
    paras.push_back(new Parameter(num));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        emit receive(parms[0]->qsMessage);
    });
}
