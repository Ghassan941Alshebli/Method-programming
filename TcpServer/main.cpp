#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

QString handleRequest(const QString& request)
{
    if (request == "MD5")
        return "MD5 function stub";

    if (request == "VIGENERE")
        return "Vigenere function stub";

    if (request == "SECANT")
        return "Secant method stub";

    if (request == "GRAPH")
        return "Graph cycle detection stub";

    return "Unknown request";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTcpServer server;

    QObject::connect(&server, &QTcpServer::newConnection, [&]() {

        QTcpSocket *client = server.nextPendingConnection();

        qDebug() << "Client connected";

        QObject::connect(client, &QTcpSocket::readyRead, [client]() {

            QString request = QString::fromUtf8(client->readAll()).trimmed();

            qDebug() << "Request:" << request;

            QString response = handleRequest(request);

            client->write(response.toUtf8());

        });

        QObject::connect(client, &QTcpSocket::disconnected, [client]() {

            qDebug() << "Client disconnected";

            client->deleteLater();

        });

    });

    if (!server.listen(QHostAddress::Any, 12345)) {

        qDebug() << "Server failed";

        return 1;
    }

    qDebug() << "Server started on port 12345";

    return a.exec();
}
