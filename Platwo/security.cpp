#include "security.h"
#include <QCryptographicHash>

QString Security::hashPassword(QString password) {
    QByteArray hash = QCryptographicHash::hash( password.toUtf8(), QCryptographicHash::Sha256);
    return hash.toHex();
}