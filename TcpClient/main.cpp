#include <QCoreApplication>
#include <QTcpSocket>

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTcpSocket socket;

    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(3000)) {
        cout << "Connection failed" << endl;
        system("pause");
        return 1;
    }

    cout << "Connected to server" << endl;

    string id;
    string name;

    cout << "Enter client id: ";
    getline(cin, id);

    cout << "Enter client name: ";
    getline(cin, name);

    QString registerRequest =
        "REGISTER|" + QString::fromStdString(id) + "|" + QString::fromStdString(name);

    socket.write(registerRequest.toUtf8());
    socket.waitForBytesWritten();

    if (socket.waitForReadyRead(3000)) {
        cout << "Server response: "
             << socket.readAll().toStdString()
             << endl;
    }

    while (true) {
        cout << "\nEnter request:" << endl;
        cout << "1. MD5" << endl;
        cout << "2. VIGENERE" << endl;
        cout << "3. SECANT" << endl;
        cout << "4. GRAPH" << endl;
        cout << "0. exit" << endl;
        cout << "> ";

        string choice;
        getline(cin, choice);

        if (choice == "0" || choice == "exit") {
            break;
        }

        string request;

        if (choice == "1")
            request = "MD5";
        else if (choice == "2")
        {
            string text;
            string key;

            cout << "Enter text: ";
            getline(cin, text);

            cout << "Enter key: ";
            getline(cin, key);

            request = "VIGENERE|" + text + "|" + key;
        }
        else if (choice == "3")
            request = "SECANT";
        else if (choice == "4")
            request = "GRAPH";
        else
            request = choice;

        socket.write(QString::fromStdString(request).toUtf8());
        socket.waitForBytesWritten();

        if (socket.waitForReadyRead(3000)) {
            cout << "Server response: "
                 << socket.readAll().toStdString()
                 << endl;
        }
        else {
            cout << "No response from server" << endl;
        }
    }

    socket.disconnectFromHost();

    cout << "Client closed" << endl;
    system("pause");

    return 0;
}