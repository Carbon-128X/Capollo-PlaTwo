#include "gameSession.h"
#include <algorithm>

GameSession::GameSession(GameMediator* game, int localPlayer, bool host) {
    game_ = game;
    localPlayer_ = localPlayer;
    host_ = host;
    gaveupBy_ = 0;
    nextSequence_ = 0;
    waitingForMove_ = false;
    lastAccepted_ = false;
    lastChangedBoard_ = false;
}

int GameSession::otherPlayer(int player) const {
    if (player == 1)
        return 2;
    else
        return 1;
}

bool GameSession::moveIsLegal(const Move& move) const {
    if (game_ == nullptr)
        return false;
    vector<Move> moves = game_->legalMoves();
    return find(moves.begin(), moves.end(), move) != moves.end();
}

vector<string> GameSession::error(const string& text) {
    lastError_ = text;
    return vector<string>{ convertError(text) };
}

vector<string> GameSession::makeLocalMove(const Move& move) {
    vector<string> toSend;
    lastAccepted_ = false;
    lastChangedBoard_ = false;
    lastError_.clear();

    if (game_ == nullptr || isOver() || !isMyTurn() || waitingForMove_)
        return toSend;
    if (!moveIsLegal(move))
        return toSend;

    if (host_) {
        if (!game_->applyMove(move))
            return toSend;

        toSend.push_back(convertMoveApplied(nextSequence_, move));
        nextSequence_++;
        lastAccepted_ = true;
        lastChangedBoard_ = true;
    }
    else {
        toSend.push_back(convertMoveRequest(nextSequence_, move));
        waitingForMove_ = true;
        lastAccepted_ = true;
    }

    return toSend;
}

vector<string> GameSession::handleMessage(const string& line) {
    vector<string> toSend;
    lastAccepted_ = false;
    lastChangedBoard_ = false;
    lastError_.clear();

    Message msg = parseMessage(line);

    if (msg.type == MSG_MOVE_REQUEST) {
        if (!host_)
            return error("Only the host can approve a move request.");
        if (isOver())
            return error("The game is already over.");
        if (msg.sequence != nextSequence_)
            return error("Unexpected move number.");
        if (game_->currentPlayer() == localPlayer_)
            return error("The guest tried to move out of turn.");
        if (!game_->applyMove(msg.move))
            return error("The guest sent an illegal move.");

        toSend.push_back(convertMoveApplied(nextSequence_, msg.move));
        nextSequence_++;
        lastAccepted_ = true;
        lastChangedBoard_ = true;
    }
    else if (msg.type == MSG_MOVE_APPLIED) {
        if (host_)
            return error("The host received an unexpected approved move.");
        if (isOver())
            return error("The game is already over.");
        if (msg.sequence != nextSequence_)
            return error("Unexpected move number.");

        int movingPlayer = game_->currentPlayer();
        if (movingPlayer == localPlayer_ && !waitingForMove_)
            return error("A local move was approved without a request.");
        if (!game_->applyMove(msg.move))
            return error("The host approved an illegal move.");

        if (movingPlayer == localPlayer_)
            waitingForMove_ = false;
        nextSequence_++;
        lastAccepted_ = true;
        lastChangedBoard_ = true;
    }
    else if (msg.type == MSG_TIMEOUT) {
        if (host_)
            return error("Only the guest should receive a timeout message.");
        if (isOver())
            return error("The game is already over.");
        if (msg.sequence != nextSequence_)
            return error("Unexpected timeout number.");
        if (msg.player != game_->currentPlayer())
            return error("Timeout player does not match the current turn.");

        gaveupBy_ = msg.player;
        waitingForMove_ = false;
        nextSequence_++;
        lastAccepted_ = true;
        lastChangedBoard_ = true;
    }
    else if (msg.type == MSG_RESIGN) {
        if (msg.player != otherPlayer(localPlayer_))
            return error("The resign message has the wrong player.");
        if (isOver())
            return toSend;

        gaveupBy_ = msg.player;
        waitingForMove_ = false;
        lastAccepted_ = true;
        lastChangedBoard_ = true;
    }
    // Chat is temporarily disabled.
    // else if (msg.type == MSG_CHAT) {
    //     lastChat_ = msg.text;
    //     lastAccepted_ = true;
    // }
    else if (msg.type == MSG_ERROR) {
        lastError_ = msg.text;
        waitingForMove_ = false;
        lastAccepted_ = true;
    }
    else {
        return error("Unknown or misplaced message.");
    }

    return toSend;
}

// Chat is temporarily disabled.
// vector<string> GameSession::sendChat(const string& text) {
//     if (text.empty())
//         return vector<string>();
//     return vector<string>{ convertChat(text) };
// }

vector<string> GameSession::timeoutCurrentPlayer() {
    vector<string> toSend;
    lastAccepted_ = false;
    lastChangedBoard_ = false;
    lastError_.clear();

    if (!host_ || game_ == nullptr || isOver())
        return toSend;

    int timedOutPlayer = game_->currentPlayer();
    gaveupBy_ = timedOutPlayer;
    toSend.push_back(convertTimeout(nextSequence_, timedOutPlayer));
    nextSequence_++;
    lastAccepted_ = true;
    lastChangedBoard_ = true;
    return toSend;
}

vector<string> GameSession::resignLocal() {
    vector<string> toSend;
    lastAccepted_ = false;
    lastChangedBoard_ = false;

    if (isOver())
        return toSend;

    gaveupBy_ = localPlayer_;
    waitingForMove_ = false;
    toSend.push_back(convertResign(localPlayer_));
    lastAccepted_ = true;
    lastChangedBoard_ = true;
    return toSend;
}

const GameMediator* GameSession::game() const {
    return game_;
}

int GameSession::localPlayer() const {
    return localPlayer_;
}

bool GameSession::isHost() const {
    return host_;
}

bool GameSession::isMyTurn() const {
    if (game_ == nullptr || isOver() || waitingForMove_)
        return false;
    return game_->currentPlayer() == localPlayer_;
}

bool GameSession::isOver() const {
    if (gaveupBy_ != 0)
        return true;
    return game_ != nullptr && game_->isGameOver();
}

int GameSession::winner() const {
    if (gaveupBy_ != 0)
        return otherPlayer(gaveupBy_);
    if (game_ == nullptr)
        return -1;
    return game_->winner();
}

int GameSession::nextSequence() const {
    return nextSequence_;
}

bool GameSession::waitingForMove() const {
    return waitingForMove_;
}

bool GameSession::lastAccepted() const {
    return lastAccepted_;
}

bool GameSession::lastChangedBoard() const {
    return lastChangedBoard_;
}

// Chat is temporarily disabled.
// string GameSession::lastChat() const {
//     return lastChat_;
// }

string GameSession::lastError() const {
    return lastError_;
}
