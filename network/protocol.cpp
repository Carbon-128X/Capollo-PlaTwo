#include "protocol.h"
#include <iomanip>
#include <sstream>

string gameTypeName(GameType type) {
    if (type == GAME_DOTS_AND_BOXES)
        return "DOTS";
    if (type == GAME_NINE_MENS_MORRIS)
        return "MORRIS";
    if (type == GAME_FANORONA)
        return "FANORONA";
    return "UNKNOWN";
}

GameType parseGameType(const string& name) {
    if (name == "DOTS")
        return GAME_DOTS_AND_BOXES;
    if (name == "MORRIS")
        return GAME_NINE_MENS_MORRIS;
    if (name == "FANORONA")
        return GAME_FANORONA;
    return GAME_UNKNOWN;
}

string convertHello(const PlayerInfo& player) {
    return "HELLO \"" + player.name + "\" \"" + player.color + "\"";
}

string convertStart(const GameConfig& config) {
    return "START " + gameTypeName(config.gameType) + " "
           + to_string(config.boardSize) + " "
           + to_string(config.timerEnabled ? 1 : 0) + " "
           + to_string(config.turnSeconds) + " "
           + "\"" + config.host.name + "\" "
           + "\"" + config.host.color + "\" "
           + "\"" + config.guest.name + "\" "
           + "\"" + config.guest.color + "\"";
}

string convertMoveRequest(int sequence, const Move& move) {
    return "MOVE_REQUEST " + to_string(sequence) + " "
           + to_string(move.from) + " " + to_string(move.to);
}

string convertMoveApplied(int sequence, const Move& move) {
    return "MOVE_APPLIED " + to_string(sequence) + " "
           + to_string(move.from) + " " + to_string(move.to);
}

// Chat is temporarily disabled.
// string convertChat(const string& text) {
//     return "CHAT \"" + text + "\"";
// }

string convertTimeout(int sequence, int player) {
    return "TIMEOUT " + to_string(sequence) + " " + to_string(player);
}

string convertResign(int player) {
    return "RESIGN " + to_string(player);
}

string convertError(const string& text) {
    return "ERROR \"" + text + "\"";
}

Message parseMessage(const string& line) {
    Message msg;
    istringstream input(line);
    string type;
    input >> type;

    if (type == "HELLO") {
        if (input >> quoted(msg.playerInfo.name)
            >> quoted(msg.playerInfo.color))
            msg.type = MSG_HELLO;
    }
    else if (type == "START") {
        string gameName;
        int timer = 0;
        if (input >> gameName
            >> msg.config.boardSize >> timer >> msg.config.turnSeconds
            >> quoted(msg.config.host.name)
            >> quoted(msg.config.host.color)
            >> quoted(msg.config.guest.name)
            >> quoted(msg.config.guest.color)) {
            msg.config.gameType = parseGameType(gameName);
            msg.config.timerEnabled = (timer != 0);
            if (msg.config.gameType != GAME_UNKNOWN && (timer == 0 || timer == 1))
                msg.type = MSG_START;
        }
    }
    else if (type == "MOVE_REQUEST") {
        if (input >> msg.sequence >> msg.move.from >> msg.move.to)
            msg.type = MSG_MOVE_REQUEST;
    }
    else if (type == "MOVE_APPLIED") {
        if (input >> msg.sequence >> msg.move.from >> msg.move.to)
            msg.type = MSG_MOVE_APPLIED;
    }
    // Chat is temporarily disabled.
    // else if (type == "CHAT") {
    //     if (input >> quoted(msg.text))
    //         msg.type = MSG_CHAT;
    // }
    else if (type == "TIMEOUT") {
        if (input >> msg.sequence >> msg.player)
            msg.type = MSG_TIMEOUT;
    }
    else if (type == "RESIGN") {
        if (input >> msg.player)
            msg.type = MSG_RESIGN;
    }
    else if (type == "ERROR") {
        if (input >> quoted(msg.text))
            msg.type = MSG_ERROR;
    }

    return msg;
}

vector<string> takeMessages(string& buffer) {
    vector<string> messages;

    while (true) {
        size_t end = buffer.find('\n');
        if (end == string::npos)
            break;

        string line = buffer.substr(0, end);
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        messages.push_back(line);
        buffer.erase(0, end + 1);
    }

    return messages;
}
