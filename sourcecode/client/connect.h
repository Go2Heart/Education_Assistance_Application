#ifndef CONNECT_H
#define CONNECT_H

#include <QObject>
#include <QTcpSocket>
#include <cstring>
#include <string>
#include <algorithm>
#include <QThread>
#include <QEventLoop>
#include <QMetaType>
#include <QVariant>
#include <QVector>
#include <QTimer>
#include "basicClass.h"

extern int UnZip(std::string x);

struct Parameter {
    enum{ STRING = 1, FILE = 2, INT = 3};
    bool isFile;
    int number, type;
    std::string s = "";
    QString qsMessage;
    std::string sMessage;
    Parameter(QString message) : qsMessage(message), type(1) {
        //qDebug()<<message;
        QByteArray byte = message.toUtf8();
        std::string tmp(byte.data(), byte.size());
        int totlen = tmp.length();
        char len[4];
        for(int i = 3; i >= 0; i--) len[i] = totlen % 256, totlen >>= 8;
        s.push_back(type);
        for(int i = 0; i <= 3; i++) s.push_back(len[i]);
        s = s + tmp;
    }
    Parameter(std::string message) : sMessage(message), type(2) {
        int totlen = message.length();
        char len[4];
        for(int i = 3; i >= 0; i--) len[i] = totlen % 256, totlen >>= 8;
        s.push_back(type);
        for(int i = 0; i <= 3; i++) s.push_back(len[i]);
        s = s + message;
    }
    Parameter(int n) : number(n), type(3) {
        std::string tmp;
        if(n != 0){
            int st[105], top = 0;
            while(n) {
                st[++top] = n % 256;
                n >>= 8;
            }
            for(int i = top; i > 0; i--) tmp.push_back(st[i]);
        } else tmp.push_back(0);
        int totlen = tmp.length();
        char len[4];
        for(int i = 3; i >= 0; i--) len[i] = totlen % 256, totlen >>= 8;
        s.push_back(type);
        for(int i = 0; i <= 3; i++) s.push_back(len[i]);
        s = s + tmp;
    }
};

Q_DECLARE_METATYPE(Parameter*)
Q_DECLARE_OPAQUE_POINTER(Parameter*)

class TcpConnector : public QObject {
    Q_OBJECT
private:
    QTcpSocket* socket = nullptr;
    bool readSize = false;
    int receiveLen = 0;
    std::string readBuf;
    QVector<Parameter*> m;
public:
    TcpConnector(QVector<Parameter*> message);
signals:
    void receive(QVariant varValue);
public slots:
    void connected();
    void readData();
};

class HomeworkPost: public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;
public:
    HomeworkPost(int id, QString desc);
signals:
    void receive(QVariant varValue);
};

class HomeworkUpload: public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    HomeworkUpload(int studentId, int classId, int homeworkId, int count, QVector<QString> fileNames,
                   QVector<std::string> fileData);
signals:
    void receive(QVariant varValue);
};

struct HomeworkResult {
    int id;
    bool finished;
    QString desc;

    HomeworkResult() {}
    HomeworkResult(int id, bool finished, QString& desc) :
        id(id),
        finished(finished),
        desc(desc)
    {}
};
Q_DECLARE_METATYPE(HomeworkResult*)
class HomeworkQuery : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    HomeworkQuery(int studentId, int classId);
signals:
    void receive(QVariant varValue);
};

class HomeworkSearch: public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    HomeworkSearch(int studentId, int classId, QString key);
signals:
    void receive(QVariant varValue);
};

struct Result {
// type: 0: x -> y 1: y -> x 2: point 3: start 4: end
// tool: 对于校区内的路:0: foot 1: bicycle
// tool：对于校区间的路:0: bus 1: shuttle
    int type, tool;
    int id;
    Result() {}
    Result(int type, int tool, int id) :
        type(type),
        tool(tool),
        id(id)
    {}
};

struct ResPackage {
    Timer timeCost;
    QVector<Result> v;
    ResPackage() {}
    ResPackage(Timer t, QVector<Result> v) :
        timeCost(t),
        v(v)
    {}
};

Q_DECLARE_METATYPE(ResPackage)

class DisQuery : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;
public:
    DisQuery(QVector<int> v);
signals:
    void receive(QVariant varValue);
};

class TimeQuery : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;
public:
    TimeQuery();
signals:
    void receive(int);
};

class TimeSpdChg : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;
public:
    TimeSpdChg(int x);
};

class LoginQuery : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;
public:
    LoginQuery(int type, QString name, QString password);
signals:
    void Id(int);
};

//classResult
struct ClassResult {
    QString name;
    QString teacher;
    QString place;
    QString time;
    int id;
    QString QQ;
    QVector<QString> fileNames;
    Timer examBegin;
    Timer examEnd;
    QString examPlace;


    ClassResult() {}
    ClassResult(QString& name, QString& teacher, QString& place, QString& time, QString& QQ, int& id, QVector<QString>& fileNames, Timer examBegin, Timer examEnd, QString examPlace) :
        name(name),
        teacher(teacher),
        place(place),
        time(time),
        QQ(QQ),
        id(id),
        fileNames(fileNames),
        examBegin(examBegin),
        examEnd(examEnd),
        examPlace(examPlace)
    {}
};
Q_DECLARE_METATYPE(ClassResult*)
class ClassQuery : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    ClassQuery(int id);
signals:
    void receive(QVariant varValue);
};

class ClassSearch: public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    ClassSearch(QString key, int type = 0);
signals:
    void receive(QVariant varValue);
};

//activityResult
struct ActivityResult {
    QString name;
    QString place;
    QString time;
    int id;
    ActivityResult() {}
    ActivityResult(QString& name, QString& place, QString& time, int& id) :
        name(name),
        place(place),
        time(time),
        id(id)
    {}
};
Q_DECLARE_METATYPE(ActivityResult*)

class ActivityQuery : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    ActivityQuery(int id);
signals:
    void receive(QVariant varValue);
};

class ActivitySearch : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    ActivitySearch(QString key, int type);
signals:
    void receive(QVariant varValue);
};

class ActivityUpload : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    ActivityUpload(QVector<QString> v, int id);
};

class FileUpload : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    FileUpload(QString id, QString descripter,std::string info, int studentId = 0, int mode = 0); // mode 0 for activity, 1 for class
};

struct FileResult {
    std::string str;
    FileResult(std::string str): str(str) {}
};
Q_DECLARE_METATYPE(FileResult*)
class FileDownload: public QObject {
    Q_OBJECT
private :
    TcpConnector* connector = nullptr;

public:
    FileDownload(QString id, QString descripter, int studentId = 0, int mode = 0); // mode 0 for activity, 1 for class

signals:
    void receive(QVariant varValue);
};

class Alarm {
public:
    enum TYPE { ONCE = 0, EVERYDAY, WEEKLY };
    Timer t;
    int frequency, id, enabled;
    QString desc, place;
    Alarm() {}
    Alarm(Timer t, int frequency, QString desc, QString place, int id, int enabled) :
        t(t), frequency(frequency), desc(desc), place(place), id(id), enabled(enabled) {}
};
Q_DECLARE_METATYPE(Alarm)

class TriggersQuery : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;
public:
    TriggersQuery();
signals:
    void receive(QVariant varValue);
};

class AlarmsQuery : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;
public:
    AlarmsQuery();
signals:
    void receive(QVariant varValue);
};

class AlarmModify : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;
public:
    AlarmModify(int id, int timer, int frequency, QString desc, QString place, int enabled);
signals:
    void receive();
};

class AlarmAdd : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;
public:
    AlarmAdd(int timer, int frequency, QString desc, QString place, int enabled);
signals:
    void receive();
};

class AlarmDel : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;
public:
    AlarmDel(int id);
signals:
    void receive();
};

class Student {
public:
    QString name, studentNumber;
    Student() {}
    Student(QString name, QString number) :
        name(name),
        studentNumber(number)
    {}
};
Q_DECLARE_METATYPE(Student)
class StudentInfoQuery : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;
public:
    StudentInfoQuery();
signals:
    void receive(QVariant x);
};

class PasswordUpd : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;
public:
    PasswordUpd(QString passwd);
};

class ClassChange : public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    ClassChange(int id, QString name, QString teacher, QString place, int classBegin, int classEnd,
                QString QQ, int examBegin, int examEnd, QString examPlace);
signals:
    void receive();
};

#endif // CONNECT_H
