#ifndef USER_H
#define USER_H
#include <QString>
#include <QJsonObject>

class User {
public:
    QString name;
    QString username;
    QString phone;
    QString email;
    QString passwordHash;

    QJsonObject toJson() const;
    static User fromJson(const QJsonObject &obj);
};

#endif // USER_H