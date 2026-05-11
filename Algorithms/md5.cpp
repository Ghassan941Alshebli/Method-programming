#include "md5.h"

#include <QCryptographicHash>

QString generateMD5(const QString& text)
{
    QByteArray hash =
        QCryptographicHash::hash(
            text.toUtf8(),
            QCryptographicHash::Md5
        );

    return hash.toHex();
}