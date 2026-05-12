#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

bool initDatabase();
bool registerUser(const QString& username, const QString& password);
bool loginUser(const QString& username, const QString& password);

#endif