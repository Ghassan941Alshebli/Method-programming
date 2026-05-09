#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

// ================= DATABASE SINGLETON =================

class Database {
private:
    static Database* instance;
    map<string, string> users;

    Database() {}

public:
    static Database* getInstance() {
        if (instance == nullptr) {
            instance = new Database();
        }
        return instance;
    }

    bool registerUser(const string& username, const string& password) {
        if (users.find(username) != users.end()) {
            return false;
        }

        users[username] = password;
        return true;
    }

    bool loginUser(const string& username, const string& password) {
        if (users.find(username) == users.end()) {
            return false;
        }

        return users[username] == password;
    }
};

Database* Database::instance = nullptr;

// ================= HELPERS =================

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(str);

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

// ================= SERVER =================

class Server {
private:
    Database* db;

public:
    Server() {
        db = Database::getInstance();
    }

    string handleRequest(const string& request) {
        vector<string> parts = split(request, '|');

        if (parts.empty()) {
            return "Invalid request";
        }

        string command = parts[0];

        if (command == "REGISTER" && parts.size() == 3) {
            return registerUser(parts[1], parts[2]);
        }

        if (command == "LOGIN" && parts.size() == 3) {
            return loginUser(parts[1], parts[2]);
        }

        if (command == "VIGENERE" && parts.size() == 3) {
            return vigenereStub(parts[1], parts[2]);
        }

        if (command == "MD5" && parts.size() == 2) {
            return md5Stub(parts[1]);
        }

        if (command == "SECANT" && parts.size() == 3) {
            return secantStub(parts[1], parts[2]);
        }

        if (command == "GRAPH" && parts.size() == 2) {
            return graphStub(parts[1]);
        }

        return "Unknown command or wrong format";
    }

private:
    string registerUser(const string& username, const string& password) {
        bool result = db->registerUser(username, password);

        if (result) {
            return "Registration successful";
        }

        return "User already exists";
    }

    string loginUser(const string& username, const string& password) {
        bool result = db->loginUser(username, password);

        if (result) {
            return "Authorization successful";
        }

        return "Invalid username or password";
    }

    string vigenereStub(const string& text, const string& key) {
        return "Vigenere function stub";
    }

    string md5Stub(const string& text) {
        return "MD5 function stub";
    }

    string secantStub(const string& x0, const string& x1) {
        return "Secant method stub";
    }

    string graphStub(const string& data) {
        return "Graph cycle detection stub";
    }
};

// ================= TEST =================

int main() {
    Server server;

    string request;

    while (true) {
        cout << "Enter request (or exit): ";
        getline(cin, request);

        if (request == "exit") break;

        cout << server.handleRequest(request) << endl;
    }

    return 0;
}
