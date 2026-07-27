#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>

class NetworkClient : public QObject {
    Q_OBJECT

public:
    explicit NetworkClient(QObject *parent = nullptr);
    bool connectToServer(const QString &ip, quint16 port);
    void disconnectFromServer();
    QTcpSocket* socket() const;

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);

private slots:
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError);

private:
    QTcpSocket *clientSocket;
};

#endif