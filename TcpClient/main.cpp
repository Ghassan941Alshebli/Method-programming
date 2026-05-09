#include <QCoreApplication>
#include <QTcpSocket>
#include <QDebug>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTcpSocket socket;

    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(3000)) {
        qDebug() << "Connection failed";
        return 1;
    }

    qDebug() << "Connected to server";

    string request;

    while (true) {
        cout << "\nEnter request:" << endl;
        cout << "1. MD5" << endl;
        cout << "2. VIGENERE" << endl;
        cout << "3. SECANT" << endl;
        cout << "4. GRAPH" << endl;
        cout << "0. exit" << endl;
        cout << "> ";

        getline(cin, request);

        if (request == "0" || request == "exit") {
            break;
        }

        if (request == "1") request = "MD5";
        else if (request == "2") request = "VIGENERE";
        else if (request == "3") request = "SECANT";
        else if (request == "4") request = "GRAPH";

        socket.write(QString::fromStdString(request).toUtf8());
        socket.waitForBytesWritten();

        if (socket.waitForReadyRead(3000)) {
            QByteArray response = socket.readAll();
            qDebug() << "Server response:" << response;
        } else {
            qDebug() << "No response from server";
        }
    }

    socket.disconnectFromHost();

    return 0;
}
