#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <QVector>
#include "user.h"
#include "gamehistory.h"
class FileManager{
public:
    static void saveUsers(const QVector<User> &users);
    static QVector<User> loadUsers();
    static QVector<GameHistory> loadHistory();
    static void saveHistory(const QVector<GameHistory> &history);
};

#endif // FILEMANAGER_H
