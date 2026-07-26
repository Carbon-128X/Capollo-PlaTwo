#pragma once
#include <string>
#include <vector>
#include "../logic/move.h"

using namespace std;

enum GameType {
    GAME_UNKNOWN,
    GAME_DOTS_AND_BOXES,
    GAME_NINE_MENS_MORRIS,
    GAME_FANORONA
};

struct PlayerInfo {
    string name;
    string color;       // a simple value such as "#ff0000"
};

struct GameConfig {
    GameType gameType = GAME_UNKNOWN;
    int boardSize = 0;          // only used by Dots and Boxes
    bool timerEnabled = false;
    int turnSeconds = 0;
    PlayerInfo host;
    PlayerInfo guest;
};

// Every network message is one line of text ending in '\n'.
enum MessageType {
    MSG_UNKNOWN,
    MSG_HELLO,
    MSG_START,
    MSG_MOVE_REQUEST,
    MSG_MOVE_APPLIED,
    // MSG_CHAT,      // Chat is temporarily disabled.
    MSG_TIMEOUT,
    MSG_RESIGN,
    MSG_ERROR
};

struct Message {
    MessageType type = MSG_UNKNOWN;
    int sequence = -1;
    int player = 0;
    Move move;
    PlayerInfo playerInfo;
    GameConfig config;
    string text;
};

string gameTypeName(GameType type);
GameType parseGameType(const string& name);

string convertHello(const PlayerInfo& player);
string convertStart(const GameConfig& config);
string convertMoveRequest(int sequence, const Move& move);
string convertMoveApplied(int sequence, const Move& move);
// string convertChat(const string& text);      // Chat is temporarily disabled.
string convertTimeout(int sequence, int player);
string convertResign(int player);
string convertError(const string& text);

Message parseMessage(const string& line);

// TCP may deliver half a message or several messages together. This function
// removes and returns only the complete lines and leaves the last partial line
// in buffer.
vector<string> takeMessages(string& buffer);
