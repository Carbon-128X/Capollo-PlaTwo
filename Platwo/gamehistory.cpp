#include "gamehistory.h"

QJsonObject GameHistory::toJson() const {
    QJsonObject obj;
    obj["username"] = username;
    obj["game"] = game;
    obj["opponent"] = opponent;
    obj["date"] = date;
    obj["role"] = role;
    obj["winner"] = winner;
    obj["score"] = score;
    return obj;
}

GameHistory GameHistory::fromJson(const QJsonObject &obj){
    GameHistory history;
    history.username = obj["username"].toString();
    history.game = obj["game"].toString();
    history.opponent = obj["opponent"].toString();
    history.date = obj["date"].toString();
    history.role = obj["role"].toString();
    history.winner = obj["winner"].toString();
    history.score = obj["score"].toString();
    return history;
}