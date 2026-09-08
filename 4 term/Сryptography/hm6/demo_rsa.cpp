#include <iostream>
#include <vector>
#include <string>
#include "rsa_keygen.h"
#include "rsa_cipher.h"

using namespace std;

void printBytes(const vector<uint8_t>& data, const string& title = "") {
    if (!title.empty()) cout << title << ": ";
    for (uint8_t b : data) {
        cout << hex << (int)b << " ";
    }
    cout << dec << endl;
}

int main() {
    cout << "RSA шифрование/дешифрование" << endl;
    
    // генерируем ключи
    RSAKeyGenerator generator;
    RSAKeyPair keys;

    while (true) {
        keys = generator.generateKeys(32, 0.9);
        
        uint64_t phi = (keys.p - 1) * (keys.q - 1);
        uint64_t check = (uint64_t)keys.e * keys.d % phi;
        
        if (check == 1) {
            break;
        }
    }
    
    // создаём шифр
    RSACipher cipher(keys.n, keys.e, keys.d);
    cout << "Размер блока: " << cipher.getBlockSize() << " байт" << endl;
    cout << endl;
        
    // шифруем текст
    string text = "Hello, RSA!";
    vector<uint8_t> plaintext(text.begin(), text.end());
    
    cout << "Исходный текст: " << text << endl;
    cout << "Длина: " << plaintext.size() << " байт" << endl;
    
    auto ciphertext = cipher.encrypt(plaintext);
    printBytes(ciphertext, "Зашифровано");
    
    // дешифруем
    auto decrypted = cipher.decrypt(ciphertext);
    string result(decrypted.begin(), decrypted.end());
    cout << "Расшифровано: " << result << endl;
    
    return 0;
}