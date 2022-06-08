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
//classResult
struct ClassResult {
    QString name;
    QString teacher;
    QString place;
    QString time;
    QString id;
    QString QQ;

    ClassResult() {}
    ClassResult(QString& name, QString& teacher, QString& place, QString& time, QString& QQ, QString& id) :
        name(name),
        teacher(teacher),
        place(place),
        time(time),
        QQ(QQ),
        id(id)
    {}
};
Q_DECLARE_METATYPE(ClassResult*)
//activityResult
struct ActivityResult {
    QString name;
    QString place;
    QString time;
    QString id;
    ActivityResult() {}
    ActivityResult(QString& name, QString& place, QString& time, QString& id) :
        name(name),
        place(place),
        time(time),
        id(id)
    {}
};
Q_DECLARE_METATYPE(ActivityResult*)
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

class ClockQuery: public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    ClockQuery(int id);
signals:
    void receive(QVariant varValue);

};

class ClassQuery: public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    ClassQuery(int id);
signals:
    void receive(QVariant varValue);
};

class ActivityQuery: public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    ActivityQuery(int id);
signals:
    void receive(QVariant varValue);
};

class ActivitySearch: public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    ActivitySearch(QString key, int type);
signals:
    void receive(QVariant varValue);
};

class ActivityUpload: public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    ActivityUpload(QVector<QString> v, int id);
};

class FileUpload: public QObject {
    Q_OBJECT
private:
    TcpConnector* connector = nullptr;

public:
    FileUpload(QString id, QString descripter,std::string info, int studentId = 0, int mode = 0); // mode 0 for activity, 1 for class
};

#endif // CONNECT_H
