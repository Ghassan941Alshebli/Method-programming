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
    bool loggedIn = false;

    while (!loggedIn)
    {
        string authChoice;
        string username;
        string email;
        string password;
        string authRequest;

        while (true)
        {
            cout << "\n1. Register" << endl;
            cout << "2. Login" << endl;
            cout << "> ";
            getline(cin, authChoice);

            if (authChoice == "1" || authChoice == "2")
                break;

            cout << "Invalid choice. Please enter 1 or 2 only." << endl;
        }

        cout << "Enter username: ";
        getline(cin, username);

        cout << "Enter email: ";
        getline(cin, email);

        cout << "Enter password: ";
        getline(cin, password);

        if (authChoice == "1")
        {
            authRequest =
                "REGISTER_USER|" +
                username + "|" +
                email + "|" +
                password;
        }
        else
        {
            authRequest =
                "LOGIN|" +
                email + "|" +
                password;
        }

        socket.write(QString::fromStdString(authRequest).toUtf8());
        socket.waitForBytesWritten();

        if (socket.waitForReadyRead(3000))
        {
            string authResponse = socket.readAll().toStdString();
            cout << "Server response: " << authResponse << endl;

            if (authResponse == "Registration successful")
            {
                cout << "Account created. Please login to continue." << endl;
            }
            else if (authResponse == "Login successful")
            {
                loggedIn = true;
            }
            else
            {
                cout << "Authentication failed. Try again." << endl;
            }
        }
        else
        {
            cout << "No response from server" << endl;
        }
    }
// يمكننا نقل هذه التعليمة التالية الى البدية لكن ساتركها هنا من اجل الاخال المستخدم


    // cout << "Enter client name: ";
    // getline(cin, name);

    // QString registerRequest =
    //     "REGISTER|" + QString::fromStdString(name);

    // socket.write(registerRequest.toUtf8());
    // socket.waitForBytesWritten();

    // if (socket.waitForReadyRead(3000)) {
    //     cout << "Server response: "
    //          << socket.readAll().toStdString()
    //          << endl;
    // }

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
        {
            string text;

            cout << "Enter text: ";
            getline(cin, text);

            request = "MD5|" + text;
        }
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
        {
            string x0;
            string x1;

            cout << "Enter x0: ";
            getline(cin, x0);

            cout << "Enter x1: ";
            getline(cin, x1);

            request = "SECANT|" + x0 + "|" + x1;
        }
        else if (choice == "4")
        {
            string edges;

            cout << "Enter graph edges: ";
            getline(cin, edges);

            request = "GRAPH|" + edges;
        }
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