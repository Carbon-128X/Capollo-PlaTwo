#include "filemanager.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

void FileManager::saveUsers(const QVector<User> &users){
    QJsonArray array;
    for(const User &user : users){
        array.append(user.toJson());
    }
    QJsonDocument document(array);
    QFile file("users.json");

    if(file.open(QIODevice::WriteOnly)){

        file.write(document.toJson());
        file.close();
    }
}

QVector<User> FileManager::loadUsers(){
    QVector<User> users;
    QFile file("users.json");

    if(!file.exists()){
        return users;
    }

    if(file.open(QIODevice::ReadOnly)){
        QByteArray data = file.readAll();
        file.close();
        QJsonDocument document = QJsonDocument::fromJson(data);
        QJsonArray array = document.array();

        for(const QJsonValue &value : array){
            users.push_back(User::fromJson(value.toObject()));
        }
    }

    return users;
}

void FileManager::saveHistory(const QVector<GameHistory> &history) {
    QJsonArray array;
    for(const GameHistory &item : history){
        array.append(item.toJson());
    }
    QFile file("history.json");

    if(file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(array).toJson());
        file.close();
    }
}

QVector<GameHistory> FileManager::loadHistory() {
    QVector<GameHistory> history;
        QFile file("history.json");

    if(!file.exists()){
        return history;
    }
    if(file.open(QIODevice::ReadOnly)) {
        QByteArray data=file.readAll();
        file.close();
        QJsonArray array=QJsonDocument::fromJson(data).array();

        for(auto value:array){
            history.push_back(GameHistory::fromJson(value.toObject()));
        }
    }

    return history;
}