#include <QString>
#include <QJsonObject>
#ifndef GAMEHISTORY_H
#define GAMEHISTORY_H

class GameHistory
{
public:
    QString username;
    QString game;
    QString opponent;
    QString date;
    QString role;
    QString winner;
    QString score;
    QJsonObject toJson() const;
    static GameHistory fromJson(const QJsonObject &obj);
};

#endif