#include "vigenere.h"

QString vigenereEncrypt(const QString& text, const QString& key)
{
    QString result;
    int keyIndex = 0;

    for (QChar ch : text) {
        if (ch.isLetter()) {
            QChar upperCh = ch.toUpper();
            QChar upperKey = key[keyIndex % key.length()].toUpper();

            int textValue = upperCh.unicode() - 'A';
            int keyValue = upperKey.unicode() - 'A';

            int encryptedValue = (textValue + keyValue) % 26;

            result += QChar('A' + encryptedValue);

            keyIndex++;
        } else {
            result += ch;
        }
    }

    return result;
}