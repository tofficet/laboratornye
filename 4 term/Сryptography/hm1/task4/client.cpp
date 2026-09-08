#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "rc4.h"

using namespace std;

vector<uint8_t> parseHex(const string& hex) {
    vector<uint8_t> result;
    for (size_t i = 0; i < hex.length(); i += 3) {
        string byteStr = hex.substr(i, 2);
        result.push_back(static_cast<uint8_t>(stoi(byteStr, nullptr, 16)));
    }
    return result;
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Socket creation failed" << endl;
        return 1;
    }
    
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    
    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Connection failed" << endl;
        return 1;
    }
    
    // ввод ключа
    string key_str;
    cout << "Enter key: ";
    getline(cin, key_str);
    vector<uint8_t> key(key_str.begin(), key_str.end());
    
    // ввод данных
    string data_str;
    cout << "Enter data: ";
    getline(cin, data_str);
    vector<uint8_t> data;

    // парсинг hex
    if (data_str.find(' ') != string::npos || data_str.find_first_not_of("0123456789ABCDEFabcdef") == string::npos) {
        data = parseHex(data_str);
    } else {
        data.assign(data_str.begin(), data_str.end());
    }
    
    // выбор режима
    cout << "Mode (0 - encrypt, 1 - decrypt): ";
    int mode;
    cin >> mode;
    
    // отправляем ключ
    uint32_t key_len = key.size();
    send(sock, &key_len, sizeof(key_len), 0);
    send(sock, key.data(), key_len, 0);
    
    // отправляем данные
    uint32_t data_len = data.size();
    send(sock, &data_len, sizeof(data_len), 0);
    send(sock, data.data(), data_len, 0);
    
    // ртправляем режим
    uint8_t mode_byte = static_cast<uint8_t>(mode);
    send(sock, &mode_byte, sizeof(mode_byte), 0);
    
    // получаем результат
    vector<uint8_t> result(data_len);
    recv(sock, result.data(), data_len, 0);
    
    // выводим результат
    cout << "Result: ";
    for (auto b : result) {
        cout << hex << (int)b << " ";
    }
    cout << dec << endl;
    
    bool is_text = true;
    for (auto b : result) {
        if (b < 32 || b > 126) {
            is_text = false;
            break;
        }
    }
    if (is_text) {
        string text(result.begin(), result.end());
        cout << "As text: " << text << endl;
    }
    
    close(sock);
    return 0;
}