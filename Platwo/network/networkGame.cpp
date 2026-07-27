#include "networkGame.h"
#include "../logic/dotsAndBoxes.h"
#include "../logic/nineMensMorris.h"
#include "../logic/fanorona.h"
#include <QNetworkInterface>

static const size_t MAX_BUFFER_SIZE = 64 * 1024;

NetworkGame::NetworkGame(QObject* parent) : QObject(parent) {
    server_ = nullptr;
    socket_ = nullptr;
    game_ = nullptr;
    session_ = nullptr;
    host_ = false;
    started_ = false;
}

NetworkGame::~NetworkGame() {
    delete session_;
    delete game_;
}

GameMediator* NetworkGame::createGame(const GameConfig& config) const {
    if (config.gameType == GAME_DOTS_AND_BOXES)
        return new DotsAndBoxes(config.boardSize, config.boardSize);
    if (config.gameType == GAME_NINE_MENS_MORRIS)
        return new NineMensMorris();
    if (config.gameType == GAME_FANORONA)
        return new Fanorona();
    return nullptr;
}

bool NetworkGame::validConfig(const GameConfig& config) const {
    if (config.gameType == GAME_UNKNOWN)
        return false;
    if (config.gameType == GAME_DOTS_AND_BOXES
        && (config.boardSize < 2 || config.boardSize > 20))
        return false;
    if (config.timerEnabled && config.turnSeconds <= 0)
        return false;
    return true;
}

bool NetworkGame::validPlayer(const PlayerInfo& player) const {
    return !player.name.empty() && player.name.size() <= 50
           && player.color.size() <= 30;
}

bool NetworkGame::startHost(const GameConfig& config,
                            const PlayerInfo& hostPlayer, quint16 port) {
    if (server_ != nullptr || socket_ != nullptr)
        return false;
    if (!validConfig(config) || !validPlayer(hostPlayer))
        return false;

    host_ = true;
    started_ = false;
    localInfo_ = hostPlayer;
    config_ = config;
    config_.host = hostPlayer;
    config_.guest = PlayerInfo();

    game_ = createGame(config_);
    if (game_ == nullptr)
        return false;
    session_ = new GameSession(game_, 1, true);

    server_ = new QTcpServer(this);
    connect(server_, &QTcpServer::newConnection,
            this, &NetworkGame::onNewConnection);

    if (!server_->listen(QHostAddress::Any, port)) {
        delete session_;
        delete game_;
        session_ = nullptr;
        game_ = nullptr;
        server_->deleteLater();
        server_ = nullptr;
        return false;
    }
    return true;
}

void NetworkGame::joinHost(const PlayerInfo& guestPlayer,
                           const QString& ip,
                           quint16 port) {
    if (server_ != nullptr || started_)
        return;

    if (!validPlayer(guestPlayer)) {
        emit connectionFailed("Invalid player information.");
        return;
    }

    host_ = false;
    localInfo_ = guestPlayer;

    // Guest is already connected but the selected color was rejected.
    // Send the new color through the same connection.
    if (socket_ != nullptr &&
        socket_->state() == QAbstractSocket::ConnectedState) {
        sendLine(convertHello(localInfo_));
        return;
    }

    // Remove an old failed connection before trying again.
    if (socket_ != nullptr) {
        socket_->deleteLater();
        socket_ = nullptr;
        buffer_.clear();
    }

    started_ = false;

    socket_ = new QTcpSocket(this);
    attachSocket(socket_);

    connect(socket_, &QTcpSocket::connected, this, [this]() {
        sendLine(convertHello(localInfo_));
    });

    socket_->connectToHost(ip, port);
}

void NetworkGame::onNewConnection() {
    while (server_->hasPendingConnections()) {
        QTcpSocket* incoming = server_->nextPendingConnection();
        if (socket_ != nullptr) {
            incoming->disconnectFromHost();
            incoming->deleteLater();
            continue;
        }

        socket_ = incoming;
        socket_->setParent(this);
        attachSocket(socket_);
        server_->close();       // one room has exactly one guest
    }
}

void NetworkGame::attachSocket(QTcpSocket* socket) {
    connect(socket, &QTcpSocket::readyRead,
            this, &NetworkGame::onReadyRead);
    connect(socket, &QTcpSocket::disconnected,
            this, &NetworkGame::onDisconnected);
    connect(socket, &QTcpSocket::errorOccurred, this,
            [this, socket](QAbstractSocket::SocketError error) {
                if (error != QAbstractSocket::RemoteHostClosedError)
                    emit connectionFailed(socket->errorString());
            });
}

void NetworkGame::sendLine(const string& line) {
    if (socket_ == nullptr
        || socket_->state() != QAbstractSocket::ConnectedState)
        return;

    string packet = line + "\n";
    socket_->write(packet.data(), (qint64)packet.size());
}

void NetworkGame::send(const vector<string>& messages) {
    for (size_t i = 0; i < messages.size(); i++)
        sendLine(messages[i]);
}

void NetworkGame::onReadyRead() {
    QByteArray bytes = socket_->readAll();
    buffer_.append(bytes.constData(), bytes.size());

    if (buffer_.size() > MAX_BUFFER_SIZE) {
        buffer_.clear();
        emit protocolError("The received message is too large.");
        socket_->disconnectFromHost();
        return;
    }

    vector<string> lines = takeMessages(buffer_);
    for (size_t i = 0; i < lines.size(); i++)
        processLine(lines[i]);
}

void NetworkGame::processLine(const string& line) {
    Message msg = parseMessage(line);

    if (msg.type == MSG_UNKNOWN) {
        sendLine(convertError("Malformed message."));
        emit protocolError("Malformed message.");
        return;
    }

    // Handshake: guest introduces itself, then host sends all game settings.
    if (!started_) {
        if (host_ && msg.type == MSG_HELLO) {
            if (!validPlayer(msg.playerInfo)) {
                sendLine(convertError("Invalid guest information."));
                return;
            }

            if (msg.playerInfo.color == config_.host.color) {
                sendLine(convertError(
                    "color is selected by host Choose another color."
                    ));
                return;
            }


            config_.guest = msg.playerInfo;
            sendLine(convertStart(config_));
            finishHandshake();
            return;
        }

        if (!host_ && msg.type == MSG_START) {
            if (!validConfig(msg.config)
                || !validPlayer(msg.config.host)
                || !validPlayer(msg.config.guest)) {
                sendLine(convertError("Invalid game settings."));
                return;
            }

            config_ = msg.config;
            game_ = createGame(config_);
            if (game_ == nullptr) {
                sendLine(convertError("Could not create the selected game."));
                return;
            }
            session_ = new GameSession(game_, 2, false);
            finishHandshake();
            return;
        }

        if (msg.type == MSG_ERROR)
            emit connectionFailed(QString::fromStdString(msg.text));
        else
            sendLine(convertError("The game has not started yet."));

        return;
    }

    if (msg.type == MSG_HELLO || msg.type == MSG_START) {
        sendLine(convertError("The handshake is already complete."));
        return;
    }

    vector<string> reply = session_->handleMessage(line);
    send(reply);

    if (session_->lastChangedBoard() && (msg.type == MSG_MOVE_REQUEST || msg.type == MSG_MOVE_APPLIED))
    {
        emit remoteMoveReceived(msg.move);
    }

    emit turnChanged(session_->game()->currentPlayer());

    if (session_->lastChangedBoard())
        emit boardChanged();

    // Chat is temporarily disabled.
    // if (msg.type == MSG_CHAT && session_->lastAccepted())
    //     emit chatReceived(QString::fromStdString(session_->lastChat()));

    if (!session_->lastError().empty())
        emit protocolError(QString::fromStdString(session_->lastError()));
}

void NetworkGame::finishHandshake() {
    started_ = true;
    emit opponentConnected();
    emit gameStarted();
}

void NetworkGame::onDisconnected() {
    started_ = false;
    emit connectionLost();
}

bool NetworkGame::playMove(const Move& move) {
    if (!started_ || session_ == nullptr)
        return false;

    vector<string> messages = session_->makeLocalMove(move);
    if (messages.empty())
        return false;

    send(messages);

    if (session_->lastChangedBoard())
    {
        emit remoteMoveReceived(move);
        emit boardChanged();
    }

    return true;
}

bool NetworkGame::timeoutCurrentPlayer() {
    if (!started_ || session_ == nullptr)
        return false;

    vector<string> messages = session_->timeoutCurrentPlayer();
    if (messages.empty())
        return false;

    send(messages);
    emit boardChanged();
    return true;
}

// Chat is temporarily disabled.
// void NetworkGame::sendChat(const QString& text) {
//     if (!started_ || session_ == nullptr)
//         return;
//     send(session_->sendChat(text.toStdString()));
// }

void NetworkGame::resign() {
    if (!started_ || session_ == nullptr)
        return;

    vector<string> messages = session_->resignLocal();
    if (messages.empty())
        return;

    send(messages);
    emit boardChanged();
}

const GameSession* NetworkGame::session() const {
    return session_;
}

const GameConfig& NetworkGame::config() const {
    return config_;
}

bool NetworkGame::isHost() const {
    return host_;
}

bool NetworkGame::isStarted() const {
    return started_;
}

quint16 NetworkGame::listeningPort() const {
    if (server_ == nullptr)
        return 0;
    return server_->serverPort();
}

QString NetworkGame::localIPv4() {
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (int i = 0; i < interfaces.size(); i++) {
        QNetworkInterface::InterfaceFlags flags = interfaces[i].flags();
        if (!flags.testFlag(QNetworkInterface::IsUp)
            || !flags.testFlag(QNetworkInterface::IsRunning)
            || flags.testFlag(QNetworkInterface::IsLoopBack))
            continue;

        QList<QNetworkAddressEntry> entries = interfaces[i].addressEntries();
        for (int j = 0; j < entries.size(); j++) {
            QHostAddress address = entries[j].ip();
            if (address.protocol() == QAbstractSocket::IPv4Protocol
                && !address.isLoopback())
                return address.toString();
        }
    }
    return "127.0.0.1";
}

