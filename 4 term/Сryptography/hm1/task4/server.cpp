#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "rc4.h"

using namespace std;

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        cerr << "Socket creation failed" << endl;
        return 1;
    }
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        cerr << "Bind failed" << endl;
        return 1;
    }
    
    listen(server_fd, 1);
    cout << "Server waiting for connection on port 8080..." << endl;
    
    int client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd < 0) {
        cerr << "Accept failed" << endl;
        return 1;
    }
    
    cout << "Client connected!" << endl;
    
    // получаем ключ от клиента
    uint32_t key_len;
    recv(client_fd, &key_len, sizeof(key_len), 0);
    
    vector<uint8_t> key(key_len);
    recv(client_fd, key.data(), key_len, 0);
    
    // получаем данные от клиента
    uint32_t data_len;
    recv(client_fd, &data_len, sizeof(data_len), 0);
    
    vector<uint8_t> data(data_len);
    recv(client_fd, data.data(), data_len, 0);
    
    // получаем режим (0 - шифрование, 1 - дешифрование)
    uint8_t mode;
    recv(client_fd, &mode, sizeof(mode), 0);
    
    cout << "Received: key=" << key_len << " bytes, data=" << data_len << " bytes, mode=" << (int)mode << endl;
    
    // обрабатываем данные
    RC4 rc4(key);
    vector<uint8_t> result = rc4.process(data);
    
    // отправляем результат клиенту
    send(client_fd, result.data(), result.size(), 0);
    
    cout << "Result sent to client" << endl;
    
    close(client_fd);
    close(server_fd);
    
    return 0;
}