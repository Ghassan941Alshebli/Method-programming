#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm> // Добавлено для использования std::find в removeUser

using namespace std;

enum class UserRole { GUEST, USER, ADMIN };

class Database {
private:
    static Database* instance;
    vector<string> users;
    map<string, UserRole> userRoles;
    map<string, string> passwords;
    
    // Приватный конструктор
    Database() {
        // При запуске сервера автоматически создаем 1 админа и 1 обычного пользователя (для тестирования)
        addUser("admin", "admin123");
        setRole("admin", UserRole::ADMIN);
        
        addUser("testuser", "1234");
    }

public:
    static Database* getInstance() {
        if (instance == nullptr) instance = new Database();
        return instance;
    }

    void addUser(const string& username, const string& password) {
        // Если пользователь уже существует, не добавляем его повторно
        if (userExists(username)) return; 
        
        users.push_back(username);
        passwords[username] = password;
        userRoles[username] = UserRole::USER;
    }

    void setRole(const string& username, UserRole role) {
        userRoles[username] = role;
    }

    UserRole getRole(const string& username) {
        return userRoles[username];
    }

    bool isAdmin(const string& username) {
        return userRoles[username] == UserRole::ADMIN;
    }

    string getRoleString(const string& username) {
        UserRole r = userRoles[username];
        if (r == UserRole::ADMIN) return "ADMIN";
        if (r == UserRole::USER)  return "USER";
        return "GUEST";
    }

    bool checkPassword(const string& u, const string& p) {
        return passwords.count(u) && passwords[u] == p;
    }

    bool userExists(const string& u) {
        return passwords.count(u) > 0;
    }

    // Функция удаления пользователя из списка и контейнеров map (используется администратором)
    bool removeUser(const string& username) {
        auto it = find(users.begin(), users.end(), username);
        if (it != users.end()) {
            users.erase(it);
            userRoles.erase(username);
            passwords.erase(username);
            return true;
        }
        return false;
    }

    // Поиск имени пользователя по ID (используется при отправке ID клиентом)
    string getUsernameById(int id) {
        if (id > 0 && id <= users.size()) {
            return users[id - 1];
        }
        return "";
    }

    // Форматирование данных для клиента в виде строки: "ID,Name,Role;"
    string getUsersForClient() {
        string result;
        int id = 1; // Так как мы используем vector, нумерацию ID начнем с 1
        for (const auto& u : users) {
            result += to_string(id) + "," + u + "," + getRoleString(u) + ";";
            id++;
        }
        return result;
    }
};

Database* Database::instance = nullptr;