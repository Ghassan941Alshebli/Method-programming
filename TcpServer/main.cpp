#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QMap>

#include "../Algorithms/vigenere.h"
#include "../Algorithms/md5.h"
#include "../Algorithms/secant.h"
#include "../Algorithms/graph.h"

#include "../Database/database.h"

struct ClientInfo
{
    int id;
    QString name;
};

QMap<QTcpSocket*, ClientInfo> clients;

int nextClientId = 1;

QString handleRequest(const QString& request)
{
    QStringList parts = request.split("|");
    QString command = parts[0];
    if (command == "REGISTER_USER")
    {
        if (parts.size() == 3)
        {
            QString username = parts[1];
            QString password = parts[2];

            if (registerUser(username, password))
                return "Registration successful";

            return "Registration failed";
        }

        return "Invalid register format";
    }

    if (command == "LOGIN")
    {
        if (parts.size() == 3)
        {
            QString username = parts[1];
            QString password = parts[2];

            if (loginUser(username, password))
                return "Login successful";

            return "Invalid username or password";
        }

        return "Invalid login format";
    }
    if (command == "MD5")
    {
        if (parts.size() == 2)
        {
            QString text = parts[1];
            return generateMD5(text);
        }

        return "Invalid MD5 format";
    }

    if (command == "VIGENERE")
    {
        if (parts.size() == 3)
        {
            QString text = parts[1];
            QString key = parts[2];

            return vigenereEncrypt(text, key);
        }

        return "Invalid Vigenere format";
    }

    if (command == "SECANT")
    {
        if (parts.size() == 3)
        {
            double x0 = parts[1].toDouble();
            double x1 = parts[2].toDouble();

            double result = secantMethod(x0, x1);

            return QString::number(result);
        }

        return "Invalid Secant format";
    }

    if (command == "GRAPH")
    {
        if (parts.size() == 2)
        {
            QString graphData = parts[1];

            bool cycle = hasCycle(graphData);

            if (cycle)
                return "Cycle detected";

            return "No cycle";
        }

        return "Invalid GRAPH format";
    }

    return "Unknown request";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!initDatabase())
    {
        qDebug() << "Database initialization failed";
        return -1;
    }

    QTcpServer server;

    QObject::connect(&server, &QTcpServer::newConnection, [&]() {

        QTcpSocket *client = server.nextPendingConnection();

        qDebug() << "New client connected";
        QObject::connect(client, &QTcpSocket::readyRead, [client]() {

            QString request = QString::fromUtf8(client->readAll()).trimmed();
            QStringList parts = request.split("|");

            ClientInfo info = clients.value(client, {0, ""});

            qDebug()
                << "Request from client:"
                << "ID =" << info.id
                << ", Name =" << info.name
                << ", Request =" << parts[0];

            QString response = handleRequest(request);

            client->write(response.toUtf8());
            client->flush();

            if (parts[0] == "LOGIN" &&
                response == "Login successful" &&
                parts.size() == 3)
            {
                ClientInfo newInfo;
                newInfo.id = nextClientId++;
                newInfo.name = parts[1];

                clients[client] = newInfo;

                qDebug()
                    << "Authenticated client:"
                    << "ID =" << newInfo.id
                    << ", Name =" << newInfo.name;
            }
        });

        QObject::connect(client,
                         &QTcpSocket::disconnected,
                         [client]() {

                             ClientInfo info = clients[client];

                             qDebug()
                                 << "Client disconnected:"
                                 << "ID =" << info.id
                                 << ", Name =" << info.name;

                             clients.remove(client);

                             client->deleteLater();
                         });
    });

    if(server.listen(QHostAddress::Any, 12345))
    {
        qDebug() << "Server started on port 12345";
    }
    else
    {
        qDebug() << "Server failed";
    }

    return a.exec();
}