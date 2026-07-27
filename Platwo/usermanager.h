#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <QVector>
#include "user.h"
#include "gamehistory.h"
class UserManager {
public:

    static QVector<User> users;
    static User currentUser;
    static void load();
    static void save();
    static bool registerUser(const User &user);
    static bool login(QString username, QString passwordHash);
    static bool usernameExists(QString username);
    static bool phoneExists(QString phone);
    static bool emailExists(QString email);
    static bool usernameExistsExceptCurrent(QString username);
    static bool phoneExistsExceptCurrent(QString phone);
    static bool emailExistsExceptCurrent(QString email);
    static bool changePassword(QString phone, QString newPasswordHash);
    static User* findUser(QString username);
    static bool updateUser(const User &user);
    static QVector<GameHistory> history;
    static void addHistory(const GameHistory &game);
    static QVector<GameHistory> getHistory(QString username, QString game);
};

#endif