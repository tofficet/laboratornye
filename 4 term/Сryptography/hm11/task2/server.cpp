#include "common.h"

int main() {
    cout << "Сервер (Боб)" << endl;
    
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 1);
    cout << "Waiting for client..." << endl;
    
    int client_fd = accept(server_fd, nullptr, nullptr);
    cout << "Client connected!" << endl;
    
    cpp_int b = 15;
    cpp_int B = powmod(G, b, P);
    
    cpp_int A = receive_bigint(client_fd);
    cout << "Received A = " << A << endl;
    
    send_bigint(client_fd, B);
    cout << "Sent B = " << B << endl;
    
    cpp_int K = powmod(A, b, P);
    cout << "\nShared secret K = " << K << endl;
    
    auto desKey = deriveKey(K, 8);
    auto aesKey = deriveKey(K, 16);
    auto marsKey = deriveKey(K, 32);
    
    cout << "\nGenerated keys:" << endl;
    cout << "DES key (8 bytes): ";
    for (auto& x : desKey) cout << hex << (int)x << " ";
    cout << dec << endl;
    
    cout << "AES-128 key (16 bytes): ";
    for (auto& x : aesKey) cout << hex << (int)x << " ";
    cout << dec << endl;
    
    cout << "MARS-256 key (32 bytes): ";
    for (auto& x : marsKey) cout << hex << (int)x << " ";
    cout << dec << endl;
    
    cout << "\nKeys successfully distributed!" << endl;
    
    close(client_fd);
    close(server_fd);
    return 0;
}