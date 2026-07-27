#include "networkserver.h"

NetworkServer::NetworkServer(QObject *parent) : QObject(parent){
    server = new QTcpServer(this);
    client = nullptr;
    connect(server, &QTcpServer::newConnection, this, &NetworkServer::onNewConnection);
}

bool NetworkServer::startServer(quint16 port) {
    return server->listen(QHostAddress::Any, port);
}

void NetworkServer::onNewConnection() {
    client = server->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, &NetworkServer::onReadyRead);
    emit clientConnected();
}

void NetworkServer::onReadyRead() {
    QString msg = QString::fromUtf8(client->readAll());
    emit messageReceived(msg);
}

void NetworkServer::sendMessage(const QString &message) {
    if(client) {
        client->write(message.toUtf8());
    }
}