#include <iostream>
#include <sstream>
#include <vector>

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

// ===== STUB FUNCTIONS =====

string vigenere(const string& text, const string& key) {
    return "Vigenere: Not implemented yet";
}

string md5(const string& text) {
    return "MD5: Not implemented yet";
}

string secant(const string& f, const string& x0, const string& x1) {
    return "Secant: Not implemented yet";
}

string graphCycle(const string& data) {
    return "Graph cycle: Not implemented yet";
}

// ===== PARSER =====

string parseInput(const string& request) {
    vector<string> parts = split(request, '|');

    if (parts.empty()) return "Invalid request";

    string command = parts[0];

    if (command == "VIGENERE" && parts.size() == 3) {
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

// ===== TEST =====

int main() {
    cout << parseInput("VIGENERE|HELLO|KEY") << endl;
    cout << parseInput("MD5|hello") << endl;
    cout << parseInput("SECANT|f(x)|1|2") << endl;
    cout << parseInput("GRAPH|1-2,2-3") << endl;

    return 0;
}
