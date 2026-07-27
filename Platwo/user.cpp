#include "user.h"

QJsonObject User::toJson() const
{
    QJsonObject obj;
    obj["name"] = name;
    obj["username"] = username;
    obj["phone"] = phone;
    obj["email"] = email;
    obj["passwordHash"] = passwordHash;
    return obj;
}

User User::fromJson(const QJsonObject &obj)
{
    User user;
    user.name = obj["name"].toString();
    user.username = obj["username"].toString();
    user.phone = obj["phone"].toString();
    user.email = obj["email"].toString();
    user.passwordHash = obj["passwordHash"].toString();
    return user;
}