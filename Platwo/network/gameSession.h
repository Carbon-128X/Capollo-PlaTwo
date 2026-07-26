#pragma once
#include <string>
#include <vector>
#include "protocol.h"
#include "../logic/gameMediator.h"

using namespace std;

class GameSession {
private:
    GameMediator* game_;
    int localPlayer_;
    bool host_;
    int gaveupBy_;
    int nextSequence_;
    bool waitingForMove_;
    bool lastAccepted_;
    bool lastChangedBoard_;
    // string lastChat_;      // Chat is temporarily disabled.
    string lastError_;

    int otherPlayer(int player) const;
    bool moveIsLegal(const Move& move) const;
    vector<string> error(const string& text);

public:
    GameSession(GameMediator* game, int localPlayer, bool host);

    vector<string> makeLocalMove(const Move& move);
    vector<string> handleMessage(const string& line);
    // vector<string> sendChat(const string& text);      // Chat is temporarily disabled.
    vector<string> timeoutCurrentPlayer();
    vector<string> resignLocal();

    // Read access for the UI and NetworkGame.
    const GameMediator* game() const;
    int localPlayer() const;
    bool isHost() const;
    bool isMyTurn() const;
    bool isOver() const;
    int winner() const;
    int nextSequence() const;
    bool waitingForMove() const;
    bool lastAccepted() const;
    bool lastChangedBoard() const;
    // string lastChat() const;      // Chat is temporarily disabled.
    string lastError() const;
};