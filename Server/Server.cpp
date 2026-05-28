#include <iostream>
#include <sstream>
#include <vector>
// Если ваш класс Database находится в Databases.cpp или .h, раскомментируйте строку ниже:
// #include "Databases.h" 

using namespace std;

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(str);

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// ===== ЗАГЛУШКИ ДЛЯ ФУНКЦИЙ АЛГОРИТМОВ =====
string vigenere(const string& text, const string& key) { return "Vigenere: Not implemented yet"; }
string md5(const string& text) { return "MD5: Not implemented yet"; }
string secant(const string& f, const string& x0, const string& x1) { return "Secant: Not implemented yet"; }
string graphCycle(const string& data) { return "Graph cycle: Not implemented yet"; }

// ===== ПАРСЕР ЗАПРОСОВ =====
string parseInput(const string& request) {
    vector<string> parts = split(request, '|');
    if (parts.empty()) return "Invalid request";

    string command = parts[0];
    Database* db = Database::getInstance(); // Подключение к базе данных

    // --- 1. АВТОРИЗАЦИЯ (РЕГИСТРАЦИЯ И ВХОД) ---
    if (command == "LOGIN" && parts.size() >= 3) {
        string username = parts[1];
        string password = parts[2];
        
        if (db->checkPassword(username, password)) {
            // При успешном входе возвращаем статус вместе с ролью пользователя
            return "Login successful|" + db->getRoleString(username);
        }
        return "Login failed: Invalid username or password";
    }
    else if (command == "REGISTER" && parts.size() >= 3) {
        string username = parts[1];
        string password = parts[2];
        
        if (db->userExists(username)) {
            return "Register failed: User already exists";
        }
        db->addUser(username, password);
        return "Register successful";
    }

    // --- 2. КОМАНДЫ АДМИНИСТРАТОРА ---
    else if (command == "GET_USERS_LIST") {
        // Получаем из базы данных список всех пользователей в формате "1,admin,ADMIN;" и возвращаем его
        return "USERS_DATA|" + db->getUsersForClient();
    }
    else if (command == "KICK_USER" && parts.size() == 2) {
        // parts[1] содержит ID удаляемого пользователя
        int targetId = stoi(parts[1]);
        string targetUsername = db->getUsernameById(targetId);
        
        if (targetUsername != "") {
            if (targetUsername == "admin") {
                return "Ошибка: Нельзя удалить главного администратора!"; // Защита главного администратора от удаления
            }
            db->removeUser(targetUsername);
            return "Пользователь удален успешно."; // Успешное удаление
        }
        return "Ошибка: Пользователь не найден.";
    }

    // --- 3. КОМАНДЫ АЛГОРИТМОВ ---
    else if (command == "VIGENERE" && parts.size() == 3) {
        return vigenere(parts[1], parts[2]);
    }
    else if (command == "MD5" && parts.size() == 2) {
        return md5(parts[1]);
    }
    else if (command == "SECANT" && parts.size() == 4) {
        return secant(parts[1], parts[2], parts[3]);
    }
    else if (command == "GRAPH" && parts.size() == 2) {
        return graphCycle(parts[1]);
    }

    return "Unknown command or wrong format";
}

// ===== ТЕСТИРОВАНИЕ =====
int main() {
    // 1. Тестирование входа под администратором (в базе по умолчанию есть admin:admin123)
    cout << "--- Testing LOGIN ---" << endl;
    cout << parseInput("LOGIN|admin|admin123") << endl; // Ответ: Login successful|ADMIN
    
    // 2. Тестирование входа обычного пользователя
    cout << parseInput("LOGIN|testuser|1234") << endl; // Ответ: Login successful|USER

    // 3. Администратор запрашивает список всех пользователей
    cout << "\n--- Testing GET_USERS_LIST ---" << endl;
    cout << parseInput("GET_USERS_LIST") << endl; // Ответ: USERS_DATA|1,admin,ADMIN;2,testuser,USER;

    // 4. Администратор удаляет второго пользователя
    cout << "\n--- Testing KICK_USER ---" << endl;
    cout << parseInput("KICK_USER|2") << endl; 
    
    // Проверка списка после удаления
    cout << parseInput("GET_USERS_LIST") << endl; // Теперь в списке должен остаться только сам администратор

    return 0;
}