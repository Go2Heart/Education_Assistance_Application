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

struct Parameter {
    bool isFile;
    std::string s = "";
    QString qsMessage;
    std::string sMessage;
    Parameter(QString message, bool isfile) : qsMessage(message), isFile(isfile) {
        //qDebug()<<message;
        QByteArray byte = message.toUtf8();
        std::string tmp(byte.data(), byte.size());
        int totlen = tmp.length();
        char len[4];
        for(int i = 3; i >= 0; i--) len[i] = totlen % 256, totlen >>= 8;
        for(int i = 0; i <= 3; i++) s.push_back(len[i]);
        s = s + tmp;
    }
    Parameter(std::string message, bool isfile) : sMessage(message), isFile(isfile) {
        int totlen = message.length();
        char len[4];
        for(int i = 3; i >= 0; i--) len[i] = totlen % 256, totlen >>= 8;
        if(isFile) len[0] += 128;
        for(int i = 0; i <= 3; i++) s.push_back(len[i]);
        s = s + message;
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

#endif // CONNECT_H
