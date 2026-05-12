#include "database.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool initDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;

    QString createTable =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL,"
        "email TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL"
        ")";

    if (!query.exec(createTable)) {
        qDebug() << "Create table failed:" << query.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully";
    return true;
}

bool registerUser(const QString& username,
                  const QString& email,
                  const QString& password)
{
    QSqlQuery checkQuery;

    checkQuery.prepare(
        "SELECT id FROM users WHERE email = :email");

    checkQuery.bindValue(":email", email);

    if (checkQuery.exec() && checkQuery.next())
    {
        qDebug() << "Email already exists";
        return false;
    }

    QSqlQuery query;

    query.prepare(
        "INSERT INTO users (username, email, password) "
        "VALUES (:username, :email, :password)");

    query.bindValue(":username", username);
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (!query.exec())
    {
        qDebug() << "Register failed:"
                 << query.lastError().text();

        return false;
    }

    return true;
}

bool loginUser(const QString& email, const QString& password)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE email = :email AND password = :password");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Login query failed:" << query.lastError().text();
        return false;
    }

    return query.next();
}