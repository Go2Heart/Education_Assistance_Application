#include "connect.h"
TcpConnector::TcpConnector(QVector<Parameter*> message) : m(message)
{
    socket = new QTcpSocket(this);
    socket->connectToHost("123.56.124.140", 8888);
    connect(socket, SIGNAL(readyRead()), SLOT(readData()));
    connect(socket, SIGNAL(connected()), SLOT(connected()));
}

void TcpConnector::connected() {
    qDebug()<<"send start";
    int sendlen = 0;
    for(int i = 0; i < m.size(); i++) {
        sendlen = sendlen + m[i]->s.length();
    }
    qDebug()<<"sendlen:"<<sendlen;
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
    qDebug()<<"send end";
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
        qDebug()<<receiveLen;
    }
    for(int i = beginIdx; i < nowbyte.length(); i++) readBuf.push_back(nowbyte[i]);
    receiveLen -= nowbyte.length() - beginIdx;

    if(!receiveLen) {
        qDebug()<<"finish recv";
        QVector<Parameter*> parms;
        int idx = 0;
        qDebug() << "readBuf len:"<<readBuf.length();
        while(idx < readBuf.length() - 1) { // 这里是否减一未定
            std::string parm;
            int len = 0;
            bool isFile = false;
            if((unsigned char)readBuf[idx] & 128) isFile = true, readBuf[idx] -= 128;
            for(int j = idx; j <= idx + 3; j++)
                len = (len << 8) + (unsigned char)readBuf[j];
            qDebug()<<"len:"<<len;
            for(int j = idx + 4; j < idx + 4 + len; j++) parm.push_back(readBuf[j]);
            idx = idx + 4 + len;
            qDebug()<<idx;
            if(isFile)
                parms.push_back(new Parameter(parm, isFile));
            else
                parms.push_back(new Parameter(QString::fromUtf8(parm.data(), parm.length()), isFile));

        }
        emit receive(QVariant::fromValue(parms));
        socket->close();
        socket->deleteLater();
    }
}
