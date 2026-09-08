#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include "twofish_wrapper.h"

using namespace std;

int main(int argc, char* argv[]) {
    ModeType selected_mode = ModeType::CTR;
    PaddingType selected_padding = PaddingType::PKCS7;
    size_t block_size_bytes = 16; // 128 бит
    size_t key_size_bytes = 32;   // 256 бит
    size_t num_threads = 6;

    // ключ и IV
    vector<uint8_t> key(key_size_bytes, 0x42);
    vector<uint8_t> iv(block_size_bytes, 0x11);

    auto algo = make_unique<TwofishWrapper>();
    CipherContext ctx(move(algo), selected_mode, selected_padding, iv);

    cout << "Twofish Algorithm Test" << endl;
    cout << "Mode: CTR | Key size: " << key_size_bytes * 8 << " bits" << endl;

    cout << "\nByte Array Test" << endl;
    vector<uint8_t> data_to_encrypt = {0x77, 0xaf, 0x5f, 0x9b, 0x21, 0x52, 0xe2, 0xe4, 0x11, 0xaa, 0xff, 0x48};
    vector<uint8_t> encrypted_data;
    vector<uint8_t> decrypted_data;

    ctx.encrypt(data_to_encrypt, encrypted_data, key, num_threads);
    ctx.decrypt(encrypted_data, decrypted_data, key, num_threads);

    if (data_to_encrypt == decrypted_data) {
        cout << "Array Status: SUCCESS" << endl;
        cout << "Original: ";
        for (auto b : data_to_encrypt) 
            cout << hex << (int)b << " ";
        cout << dec << endl;
        cout << "Decrypted: ";
        for (auto b : decrypted_data) 
            cout << hex << (int)b << " ";
        cout << dec << endl;
    } else {
        cout << "Array Status: FAILURE" << endl;
    }

    cout << "\nFile Test" << endl;
    string input_filename = "kek.jpg";
    if (argc > 1) 
        input_filename = argv[1];

    ifstream check_file(input_filename, ios::binary);
    if (!check_file.good()) {
        cerr << "Skip file test: File '" << input_filename << "' not found." << endl;
        return 0;
    }
    check_file.close();

    size_t dot_pos = input_filename.find_last_of('.');
    string base_name = (dot_pos != string::npos) ? input_filename.substr(0, dot_pos) : input_filename;
    string extension = (dot_pos != string::npos) ? input_filename.substr(dot_pos) : "";

    string enc_file = base_name + "_enc" + extension;
    string dec_file = base_name + "_dec" + extension;

    try {
        auto f_enc = ctx.encryptFileAsync(input_filename, enc_file, key, num_threads);
        f_enc.get();

        auto f_dec = ctx.decryptFileAsync(enc_file, dec_file, key, num_threads);
        f_dec.get();

        ifstream f1(input_filename, ios::binary);
        ifstream f2(dec_file, ios::binary);
        vector<uint8_t> v1((istreambuf_iterator<char>(f1)), {});
        vector<uint8_t> v2((istreambuf_iterator<char>(f2)), {});

        if (v1 == v2 && !v1.empty()) {
            cout << "SUCCESS: Files are identical." << endl;
            cout << "Original size: " << v1.size() << " bytes" << endl;
            cout << "Decrypted size: " << v2.size() << " bytes" << endl;
        } else {
            cout << "FAILURE: Decrypted file differs!" << endl;
            cout << "Original size: " << v1.size() << " bytes" << endl;
            cout << "Decrypted size: " << v2.size() << " bytes" << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Critical Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}