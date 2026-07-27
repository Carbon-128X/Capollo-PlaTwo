#include "networkclient.h"

NetworkClient::NetworkClient(QObject *parent) : QObject(parent) {
    clientSocket = new QTcpSocket(this);

    connect(clientSocket, &QTcpSocket::connected, this, &NetworkClient::onConnected);
    connect(clientSocket, &QTcpSocket::disconnected, this, &NetworkClient::onDisconnected);
    connect(clientSocket, &QTcpSocket::errorOccurred, this, &NetworkClient::onErrorOccurred);
}

bool NetworkClient::connectToServer(const QString &ip, quint16 port) {
    clientSocket->connectToHost(ip, port);
    return clientSocket->waitForConnected(3000);
}

void NetworkClient::disconnectFromServer() {
    clientSocket->disconnectFromHost();
}

QTcpSocket *NetworkClient::socket() const {
    return clientSocket;
}

void NetworkClient::onConnected() {
    emit connected();
}

void NetworkClient::onDisconnected() {
    emit disconnected();
}

void NetworkClient::onErrorOccurred(QAbstractSocket::SocketError) {
    emit connectionError(clientSocket->errorString());
}