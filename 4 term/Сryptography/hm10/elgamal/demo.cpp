#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "elgamal.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "ElGamal Algorithm Test" << endl;

    // генерация ключей
    cpp_int min_p = cpp_int(1) << 31;
    cpp_int max_p = (cpp_int(1) << 32) - 1;

    el_gamal_public_key pub;
    el_gamal_private_key priv;

    cout << "Generating keys..." << endl;
    el_gamal::generate_keys(min_p, max_p, pub, priv);

    // тест на массиве байт
    cout << "\nByte Array Test" << endl;
    vector<uint8_t> data = {0x77, 0xaf, 0x5f, 0x9b, 0x21, 0x52, 0xe2, 0xe4, 0x11, 0xaa, 0xff, 0x48};

    cpp_int m = 0;
    for (uint8_t b : data)
        m = (m << 8) | b;

    cout << "Message (hex): ";
    for (uint8_t b : data) 
        cout << hex << (int)b << " ";
    cout << dec << endl;

    vector<uint8_t> recovered;

    if (m < pub.p) {
        // сообщение помещается — шифруем напрямую
        el_gamal_ciphertext cipher = el_gamal::encrypt(m, pub);
        cout << "Encrypted: a=" << cipher.a << ", b=" << cipher.b << endl;

        cpp_int decrypted = el_gamal::decrypt(cipher, pub, priv);
        cout << "Decrypted: " << decrypted << endl;

        // восстанавливаем байты
        cpp_int temp = decrypted;
        while (temp > 0) {
            recovered.push_back(static_cast<uint8_t>(temp & 0xFF));
            temp >>= 8;
        }
        reverse(recovered.begin(), recovered.end());
    } else {
        // сообщение слишком большое — используем временный файл
        string temp_in = "temp_input.bin";
        string temp_enc = "temp_encrypted.bin";
        string temp_dec = "temp_decrypted.bin";
        
        // записываем массив во временный файл
        ofstream fout(temp_in, ios::binary);
        fout.write(reinterpret_cast<const char*>(data.data()), data.size());
        fout.close();
        
        // шифруем файл
        el_gamal::encrypt_file(temp_in, temp_enc, pub);
        
        // читаем зашифрованный файл и показываем первые a и b
        ifstream fin(temp_enc, ios::binary);
        size_t original_size;
        fin.read(reinterpret_cast<char*>(&original_size), sizeof(original_size));
        
        // читаем первый блок (a и b)
        size_t a_len, b_len;
        fin.read(reinterpret_cast<char*>(&a_len), sizeof(a_len));
        vector<uint8_t> a_bytes(a_len);
        fin.read(reinterpret_cast<char*>(a_bytes.data()), a_len);
        fin.read(reinterpret_cast<char*>(&b_len), sizeof(b_len));
        vector<uint8_t> b_bytes(b_len);
        fin.read(reinterpret_cast<char*>(b_bytes.data()), b_len);
        fin.close();
        
        // восстанавливаем a и b
        cpp_int a, b;
        boost::multiprecision::import_bits(a, a_bytes.begin(), a_bytes.end());
        boost::multiprecision::import_bits(b, b_bytes.begin(), b_bytes.end());
        
        cout << "Encrypted: a = " << a << ", b = " << b << endl;
        
        // дешифруем файл
        el_gamal::decrypt_file(temp_enc, temp_dec, pub, priv);
        
        // читаем результат
        ifstream fin2(temp_dec, ios::binary);
        recovered.assign((istreambuf_iterator<char>(fin2)), {});
        fin2.close();
        
        // удаляем временные файлы
        remove(temp_in.c_str());
        remove(temp_enc.c_str());
        remove(temp_dec.c_str());
        
        // собираем расшифрованные байты в число
        cpp_int decrypted = 0;
        for (uint8_t b : recovered)
            decrypted = (decrypted << 8) | b;
        
        cout << "Decrypted: " << decrypted << endl;
    }

    // проверка
    if (recovered == data)
        cout << "Array Status: SUCCESS" << endl;
    else
        cout << "Array Status: FAILURE" << endl;

    // тест на файле
    cout << "\nFile Test" << endl;
    string input_file = "kek.jpg";
    if (argc > 1) 
        input_file = argv[1];

    ifstream check(input_file, ios::binary);
    if (!check.good()) {
        cerr << "File '" << input_file << "' not found." << endl;
        return 0;
    }
    check.close();

    size_t dot_pos = input_file.find_last_of('.');
    string base_name = (dot_pos != string::npos) ? input_file.substr(0, dot_pos) : input_file;
    string extension = (dot_pos != string::npos) ? input_file.substr(dot_pos) : "";

    string enc_file = base_name + "_enc" + extension;
    string dec_file = base_name + "_dec" + extension;

    try {
        cout << "Encrypting file..." << endl;
        el_gamal::encrypt_file(input_file, enc_file, pub);

        cout << "Decrypting file..." << endl;
        el_gamal::decrypt_file(enc_file, dec_file, pub, priv);

        ifstream f1(input_file, ios::binary);
        ifstream f2(dec_file, ios::binary);
        vector<uint8_t> v1((istreambuf_iterator<char>(f1)), {});
        vector<uint8_t> v2((istreambuf_iterator<char>(f2)), {});

        if (v1 == v2 && !v1.empty()) {
            cout << "File Status: SUCCESS" << endl;
            cout << "Original size: " << v1.size() << " bytes" << endl;
            cout << "Decrypted size: " << v2.size() << " bytes" << endl;
        } else
            cout << "File Status: FAILURE" << endl;
    }
    
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}