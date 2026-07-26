#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include "gameSession.h"
#include "logic/gameMediator.h"

// Host:
//   startHost(config, player, port)
// Guest:
//   joinHost(player, ip, port)

// Both sides wait for gameStarted() before opening the board window.

class NetworkGame : public QObject {
    Q_OBJECT

private:
    QTcpServer* server_;
    QTcpSocket* socket_;
    GameMediator* game_;
    GameSession* session_;
    GameConfig config_;
    PlayerInfo localInfo_;
    bool host_;
    bool started_;
    string buffer_;

    GameMediator* createGame(const GameConfig& config) const;
    bool validConfig(const GameConfig& config) const;
    bool validPlayer(const PlayerInfo& player) const;
    void attachSocket(QTcpSocket* socket);
    void send(const vector<string>& messages);
    void sendLine(const string& line);
    void processLine(const string& line);
    void finishHandshake();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

public:
    explicit NetworkGame(QObject* parent = nullptr);
    ~NetworkGame();

    bool startHost(const GameConfig& config, const PlayerInfo& hostPlayer, quint16 port);
    void joinHost(const PlayerInfo& guestPlayer, const QString& ip, quint16 port);

    bool playMove(const Move& move);
    bool timeoutCurrentPlayer();
    // void sendChat(const QString& text);      // Chat is temporarily disabled.
    void resign();

    const GameSession* session() const;
    const GameConfig& config() const;
    bool isHost() const;
    bool isStarted() const;
    quint16 listeningPort() const;

    static QString localIPv4();

signals:
    void opponentConnected();
    void gameStarted();
    void boardChanged();
    // void chatReceived(QString text);      // Chat is temporarily disabled.
    void protocolError(QString text);
    void connectionLost();
    void connectionFailed(QString reason);
    void remoteMoveReceived(const Move &move);
    void turnChanged(int player);
};
