#include "connect.h"

int UnZip(std::string x) {
    int y = 0;
    for(int i = 0; i < x.length(); i++) y = (y << 8) + (unsigned char)x[i];
    return y;
}

TcpConnector::TcpConnector(QVector<Parameter*> message) : m(message)
{
    socket = new QTcpSocket(this);
    socket->connectToHost("82.157.164.204", 43434);
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

DisQuery::DisQuery(QVector<int> v) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(3));
    paras.push_back(new Parameter(v.size()));
    for(int i = 0; i < v.size(); i++) paras.push_back(new Parameter(v[i]));
    paras.push_back(new Parameter(1));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        Timer nowTimer;
        nowTimer.FromZip(parms[0]->number);
        int size = parms[1]->number;
        QVector<Result> v;
        for(int i = 1, j = 2; i <= size; i++, j += 3)
            v.push_back(Result(parms[j]->number, parms[j + 1]->number, parms[j + 2]->number));
        emit receive(QVariant::fromValue(ResPackage(nowTimer, v)));
    });
}

TimeQuery::TimeQuery() {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(4));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
       QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
       emit receive(parms[0]->number);
    });
}

TimeSpdChg::TimeSpdChg(int x) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(5));
    paras.push_back(new Parameter(x));
    connector = new TcpConnector(paras);
}

LoginQuery::LoginQuery(int type, QString name, QString password) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(6));
    paras.push_back(new Parameter(type));
    paras.push_back(new Parameter(name));
    paras.push_back(new Parameter(password));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
       QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
       emit Id(parms[0]->number);
    });
}


ClassQuery::ClassQuery(int id) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(7));
    paras.push_back(new Parameter(id));
    //qDebug() << id;
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
       QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
       QVector<ClassResult*> v;
       for(int i = 0; i < parms.size(); i += 4) {
           v.push_back(new ClassResult( parms[i]->qsMessage, parms[i + 1]->qsMessage, parms[i + 2]->qsMessage, parms[i + 3]->qsMessage));
       }
       //qDebug()<<"ClassQuery";
       emit receive(QVariant::fromValue(v));
    });
}

ActivityQuery::ActivityQuery(int id) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(8));
    paras.push_back(new Parameter(id));
    qDebug() << "activity id: " << id;
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
       QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
       QVector<ActivityResult*> v;
       for(int i = 0; i < parms.size(); i += 3) {
           v.push_back(new ActivityResult( parms[i]->qsMessage, parms[i + 1]->qsMessage, parms[i + 2]->qsMessage));
       }
       qDebug() << "activity size: " << v.size();
       emit receive(QVariant::fromValue(v));
    });
}

activitySearch::activitySearch(QString name, int type) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(10));
    paras.push_back(new Parameter(type));
    paras.push_back(new Parameter(name));
    connector = new TcpConnector(paras);
    connect(connector, &TcpConnector::receive, this, [=](QVariant varValue) {
        QVector<Parameter*> parms = varValue.value<QVector<Parameter*>>();
        QVector<ActivityResult*> v;
        for(int i = 0; i < parms.size(); i += 3) {
            v.push_back(new ActivityResult( parms[i]->qsMessage, parms[i + 1]->qsMessage, parms[i + 2]->qsMessage));
        }
        emit receive(QVariant::fromValue(v));
    });

}

activityUpload::activityUpload(QVector<QString> v, int id) {
    QVector<Parameter*> paras;
    paras.push_back(new Parameter(11));
    paras.push_back(new Parameter(v[2])); //place
    paras.push_back(new Parameter(v[0])); //name
    paras.push_back(new Parameter(1)); //type
    paras.push_back(new Parameter(v[3])); //time
    qDebug() << "activity time: " << v[3];
    paras.push_back(new Parameter(id)); //content
    connector = new TcpConnector(paras);
}
