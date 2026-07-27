#ifndef SECURITY_H
#define SECURITY_H
#include <QString>

class Security
{
public:
    static QString hashPassword(QString password);
};

#endif