#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Client {
private:
    int id;
    string name;

public:
    Client(int clientId, const string& clientName) {
        id = clientId;
        name = clientName;
    }

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }
};

class Server {
private:
    vector<Client> clients;

public:
    void connectClient(int id, const string& name) {
        clients.push_back(Client(id, name));
        cout << "Client connected: " << name << endl;
    }

    void disconnectClient(int id) {
        for (auto it = clients.begin(); it != clients.end(); ++it) {
            if (it->getId() == id) {
                cout << "Client disconnected: " << it->getName() << endl;
                clients.erase(it);
                return;
            }
        }

        cout << "Client not found" << endl;
    }

    void showClients() {
        cout << "Connected clients:" << endl;

        for (const Client& client : clients) {
            cout << "ID: " << client.getId()
                 << ", Name: " << client.getName() << endl;
        }
    }

    void handleRequest(int clientId, const string& request) {
        cout << "Request from client " << clientId << ": " << request << endl;

        if (request == "MD5") {
            cout << "Response: MD5 function stub" << endl;
        }
        else if (request == "VIGENERE") {
            cout << "Response: Vigenere function stub" << endl;
        }
        else if (request == "SECANT") {
            cout << "Response: Secant method stub" << endl;
        }
        else if (request == "GRAPH") {
            cout << "Response: Graph cycle detection stub" << endl;
        }
        else {
            cout << "Response: Unknown request" << endl;
        }
    }
};

int main() {
    Server server;

    int choice;

    while (true) {
        cout << "\n===== SERVER MENU =====" << endl;
        cout << "1. Connect client" << endl;
        cout << "2. Disconnect client" << endl;
        cout << "3. Show clients" << endl;
        cout << "4. Send request" << endl;
        cout << "0. Exit" << endl;
        cout << "Choose option: ";
        cin >> choice;

        if (choice == 0) {
            break;
        }

        if (choice == 1) {
            int id;
            string name;

            cout << "Enter client id: ";
            cin >> id;

            cout << "Enter client name: ";
            cin >> name;

            server.connectClient(id, name);
        }
        else if (choice == 2) {
            int id;

            cout << "Enter client id to disconnect: ";
            cin >> id;

            server.disconnectClient(id);
        }
        else if (choice == 3) {
            server.showClients();
        }
        else if (choice == 4) {
            int id;
            string request;

            cout << "Enter client id: ";
            cin >> id;

            cout << "Enter request (MD5, VIGENERE, SECANT, GRAPH): ";
            cin >> request;

            server.handleRequest(id, request);
        }
        else {
            cout << "Wrong option" << endl;
        }
    }

    return 0;
}
