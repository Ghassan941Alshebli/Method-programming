#include <iostream>
#include <string>

using namespace std;

string vigenereEncrypt(string text, string key)
{
    string result = "";
    int keyIndex = 0;

    for (int i = 0; i < text.length(); i++)
    {
        if (isalpha(text[i]))
        {
            char textChar = toupper(text[i]);
            char keyChar = toupper(key[keyIndex % key.length()]);

            int textValue = textChar - 'A';
            int keyValue = keyChar - 'A';

            int encryptedValue = (textValue + keyValue) % 26;

            result += char(encryptedValue + 'A');

            keyIndex++;
        }
        else
        {
            result += text[i];
        }
    }

    return result;
}

int main()
{
    string text, key;

    cout << "Enter text: ";
    getline(cin, text);

    cout << "Enter key: ";
    cin >> key;

    string encryptedText = vigenereEncrypt(text, key);

    cout << "Encrypted text: " << encryptedText << endl;

    return 0;
}
