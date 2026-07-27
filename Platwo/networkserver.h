#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class NetworkServer : public QObject {
    Q_OBJECT

public:
    explicit NetworkServer(QObject *parent = nullptr);
    bool startServer(quint16 port);
    void sendMessage(const QString &message);

private:
    QTcpServer *server;
    QTcpSocket *client;

signals:
    void clientConnected();
    void messageReceived(QString message);

private slots:
    void onNewConnection();
    void onReadyRead();
};

#endif