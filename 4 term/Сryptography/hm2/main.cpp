#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <random>
#include <chrono>
#include "include/encryption_and_decryption.h"
#include "include/des.h"
#include "include/triple_des.h"

using namespace std;

vector<uint8_t> generateRandomKey(size_t size) {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(0, 255);
    
    vector<uint8_t> key(size);
    for (size_t i = 0; i < size; i++) {
        key[i] = (uint8_t)dis(gen);
    }
    return key;
}

void saveKeyToFile(const vector<uint8_t>& key, const string& filename = "key.txt") {
    ofstream file(filename);
    for (uint8_t b : key) {
        file << hex << setw(2) << setfill('0') << (int)b;
    }
    file << endl;
}

vector<uint8_t> loadKeyFromFile(const string& filename = "key.txt") {
    ifstream file(filename);
    string hex;
    file >> hex;
    
    vector<uint8_t> key;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteStr = hex.substr(i, 2);
        uint8_t byte = (uint8_t)strtol(byteStr.c_str(), nullptr, 16);
        key.push_back(byte);
    }
    return key;
}

bool keyFileExists(const string& filename = "key.txt") {
    ifstream file(filename);
    return file.good();
}

void printHex(const vector<uint8_t>& data) {
    for (uint8_t b : data) {
        cout << hex << setw(2) << setfill('0') << (int)b;
    }
    cout << dec << setfill(' ');
}

vector<uint8_t> hexToBytes(const string& hex) {
    vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteStr = hex.substr(i, 2);
        uint8_t byte = (uint8_t)strtol(byteStr.c_str(), nullptr, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

vector<uint8_t> readFile(const string& path) {
    ifstream file(path, ios::binary);
    return vector<uint8_t>((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

void writeFile(const string& path, const vector<uint8_t>& data) {
    ofstream file(path, ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

vector<uint8_t> parseInput(const string& input) {
    if (input.length() >= 2 && input[0] == '0' && input[1] == 'x') {
        return hexToBytes(input.substr(2));
    }
    ifstream test(input, ios::binary);
    if (test) {
        test.close();
        return readFile(input);
    }
    return vector<uint8_t>(input.begin(), input.end());
}

IBlockCipher* createCipher(const string& algo) {
    if (algo == "des") return new DESCipher();
    if (algo == "3eee3") return new TripleDES(TripleDESMode::EEE3);
    if (algo == "3ede3") return new TripleDES(TripleDESMode::EDE3);
    if (algo == "3eee2") return new TripleDES(TripleDESMode::EEE2);
    if (algo == "3ede2") return new TripleDES(TripleDESMode::EDE2);
    return nullptr;
}

size_t getKeySize(const string& algo) {
    if (algo == "des") return 8;
    if (algo == "3eee2" || algo == "3ede2") return 16;
    if (algo == "3eee3" || algo == "3ede3") return 24;
    return 0;
}

void printUsage() {
    cout << "Usage:\n";
    cout << "  ./crypto <algo> <op> <input> [output]\n\n";
    cout << "Algorithms:\n";
    cout << "  des, 3eee3, 3ede3, 3eee2, 3ede2\n\n";
    cout << "Operations:\n";
    cout << "  enc, dec\n\n";
    cout << "Key file: key.txt (auto-generated on first run)\n\n";
    cout << "Examples:\n";
    cout << "  ./crypto des enc \"Hello\"\n";
    cout << "  ./crypto des dec 0x7c280a5963ec882d\n";
    cout << "  ./crypto des enc input.txt output.enc\n";
    cout << "  ./crypto 3ede2 enc \"Secret\"\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printUsage();
        return 1;
    }

    string algo = argv[1];
    string op = argv[2];
    string input = argv[3];
    string output = (argc == 5) ? argv[4] : "";

    size_t keySize = getKeySize(algo);
    if (keySize == 0) {
        cerr << "Unknown algorithm: " << algo << endl;
        return 1;
    }

    vector<uint8_t> key;
    string keyFile = "key_" + algo + ".txt";
    
    if (keyFileExists(keyFile)) {
        key = loadKeyFromFile(keyFile);
    } else {
        key = generateRandomKey(keySize);
        saveKeyToFile(key, keyFile);
    }

    IBlockCipher* cipher = createCipher(algo);
    if (!cipher) {
        cerr << "Unknown algorithm: " << algo << endl;
        return 1;
    }

    CipherContext context(cipher);
    vector<uint8_t> data = parseInput(input);

    bool isFile = false;
    if (input.length() >= 2 && input[0] == '0' && input[1] == 'x') {
        isFile = false;
    } else {
        ifstream test(input, ios::binary);
        if (test) {
            test.close();
            isFile = true;
        }
    }

    cout << endl;
    cout << "Algorithm: " << algo << endl;
    cout << "Operation: " << op << endl;
    cout << "Key: ";
    printHex(key);
    cout << endl;
    cout << "Data size: " << data.size() << " bytes" << endl;
    if (isFile) cout << "Input: file" << endl;
    cout << endl;

    vector<uint8_t> result;
    
    try {
        if (op == "enc") {
            result = context.encrypt(data, key);
            cout << "Encrypted: " << result.size() << " bytes" << endl;
            
            if (!isFile && result.size() <= 32) {
                cout << "HEX: ";
                printHex(result);
                cout << endl;
            }
            
        } else if (op == "dec") {
            result = context.decrypt(data, key);
            cout << "Decrypted: " << result.size() << " bytes" << endl;
            
            if (!isFile && result.size() <= 32) {
                cout << "HEX: ";
                printHex(result);
                cout << endl;
            }
            
            if (!isFile && result.size() <= 100) {
                bool isText = true;
                for (uint8_t b : result) {
                    if (b < 32 && b != 10 && b != 13 && b != 9) {
                        isText = false;
                        break;
                    }
                }
                if (isText && !result.empty()) {
                    cout << "\nText: " << string(result.begin(), result.end()) << endl;
                }
            }
            
        } else {
            cerr << "Unknown operation: " << op << endl;
            return 1;
        }

        if (!output.empty()) {
            writeFile(output, result);
            cout << "\nSaved to: " << output << endl;
        }
        
    } catch (const exception& e) {
        cerr << "\nERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}