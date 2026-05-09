#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Database {
private:
    static Database* instance;
    vector<string> users;

    Database() {}

public:
    static Database* getInstance() {
        if (instance == nullptr) {
            instance = new Database();
        }
        return instance;
    }

    void addUser(const string& username) {
        users.push_back(username);
    }

    vector<string> getUsers() {
        return users;
    }
};

Database* Database::instance = nullptr;

int main() {
    Database* db = Database::getInstance();

    string username;

    while (true) {

        cout << "Enter username (or exit): ";
        cin >> username;

        if (username == "exit") {
            break;
        }

        db->addUser(username);
    }

    vector<string> users = db->getUsers();

    cout << "\nUsers in database:\n";

    for (const string& user : users) {
        cout << user << endl;
    }

    return 0;
}
