#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QMap>
#include "../Algorithms/vigenere.h"
    struct ClientInfo
{
    int id;
    QString name;
};

QMap<QTcpSocket*, ClientInfo> clients;

QString handleRequest(const QString& request)
{
    QStringList parts = request.split("|");
    QString command = parts[0];

    if (command == "MD5")
        return "MD5 function stub";

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
        return "Secant method stub";

    if (command == "GRAPH")
        return "Graph cycle detection stub";

    return "Unknown request";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTcpServer server;

    QObject::connect(&server, &QTcpServer::newConnection, [&]() {

        QTcpSocket *client = server.nextPendingConnection();

        qDebug() << "New client connected";

        QObject::connect(client, &QTcpSocket::readyRead, [client]() {

            QString request =
                QString::fromUtf8(client->readAll()).trimmed();

            QStringList parts = request.split("|");

            if(parts[0] == "REGISTER")
            {
                ClientInfo info;
                info.id = parts[1].toInt();
                info.name = parts[2];

                clients[client] = info;

                qDebug()
                    << "Registered client:"
                    << "ID =" << info.id
                    << ", Name =" << info.name;

                client->write("REGISTERED");
            }
            else
            {
                ClientInfo info = clients[client];

                qDebug()
                    << "Request from client:"
                    << "ID =" << info.id
                    << ", Name =" << info.name
                    << ", Request =" << parts[0];

               QString response = handleRequest(request);

                client->write(response.toUtf8());
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

