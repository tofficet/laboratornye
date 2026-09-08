#include "common.h"

int main() {
    cout << "Клиент (Алиса)" << endl;
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);
    
    connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));
    cout << "Connected to server!" << endl;
    
    cpp_int a = 6;
    cpp_int A = powmod(G, a, P);
    
    send_bigint(sock, A);
    cout << "Sent A = " << A << endl;
    
    cpp_int B = receive_bigint(sock);
    cout << "Received B = " << B << endl;
    
    cpp_int K = powmod(B, a, P);
    cout << "\nShared secret K = " << K << endl;
    
    auto desKey = deriveKey(K, 8);
    auto aesKey = deriveKey(K, 16);
    auto marsKey = deriveKey(K, 32);
    
    cout << "\nGenerates keys:" << endl;
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
    
    close(sock);
    return 0;
}