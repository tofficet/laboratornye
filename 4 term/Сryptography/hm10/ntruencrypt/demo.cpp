#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "ntruencrypt.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "NTRUEncrypt File Demonstration" << endl;
    
    ntru_cipher cipher;

    cipher.generate_keys();

    cout << "\nFile Encryption Test" << endl;
    string input_file = "test.txt";
    if (argc > 1) {
        input_file = argv[1];
    }

    size_t dot_pos = input_file.find_last_of('.');
    string base_name = (dot_pos != string::npos) ? input_file.substr(0, dot_pos) : input_file;
    string extension = (dot_pos != string::npos) ? input_file.substr(dot_pos) : "";

    string enc_file = base_name + "_enc" + extension;
    string dec_file = base_name + "_dec" + extension;

    try {
        cipher.encrypt_file(input_file, enc_file);
        cipher.decrypt_file(enc_file, dec_file);

        ifstream f1(input_file, ios::binary);
        ifstream f2(dec_file, ios::binary);
        vector<uint8_t> v1((istreambuf_iterator<char>(f1)), {});
        vector<uint8_t> v2((istreambuf_iterator<char>(f2)), {});

        if (v1 == v2 && !v1.empty()) {
            cout << "Integrity Check: SUCCESS (Files are identical)" << endl;
            cout << "Original size: " << v1.size() << " bytes" << endl;
            cout << "Decrypted size: " << v2.size() << " bytes" << endl;
        } else {
            cout << "Integrity Check: FAILURE (Files differ or empty)" << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Error occurred: " << e.what() << endl;
        return 1;
    }

    return 0;
}