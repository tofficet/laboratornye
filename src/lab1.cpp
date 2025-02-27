#include <fstream>
#include <iostream>

class encoder {
   private:
    unsigned char* key_;
    size_t key_size_;
    unsigned char* state_;
    int i, j, t;

    void KSA();
    unsigned char PRGA();

   public:
    encoder(unsigned char const* key, size_t key_size);
    ~encoder();
    void encode(char const* input, char const* output, bool flag);
    void mutator(unsigned char const* key, size_t key_size);
};

encoder::encoder(unsigned char const* key, size_t key_size) {
    std::cout << "Constructor working...\n";
    state_ = new unsigned char[256];
    key_size_ = key_size;
    key_ = new unsigned char[key_size_];
    i = 0;
    j = 0;

    for (size_t index = 0; index < key_size_; ++index) {
        key_[index] = key[index];
    }
}

encoder::~encoder() {
    std::cout << "Destructor working...\n";
    delete[] key_;
    delete[] state_;
}

void encoder::mutator(const unsigned char* new_key, size_t new_key_size) {
    if (new_key_size > 256) {
        std::cout << "Error\n";
        return;
    }

    delete[] key_;
    key_size_ = new_key_size;
    key_ = new unsigned char[key_size_];

    for (size_t i = 0; i < key_size_; ++i) {
        key_[i] = new_key[i];
    }

    std::cout << "Mutator completed.\n";
    KSA();
}

void encoder::KSA() {
    for (int i = 0; i < 256; ++i) {
        state_[i] = i;
    }
    j = 0;
    for (int i = 0; i < 256; ++i) {
        j = (j + state_[i] + key_[i % key_size_]) % 256;

        unsigned char temp = state_[i];
        state_[i] = state_[j];
        state_[j] = temp;
    }
    i = 0;
    j = 0;
}

unsigned char encoder::PRGA() {
    i = (i + 1) % 256;
    j = (j + state_[i]) % 256;

    unsigned char temp = state_[i];
    state_[i] = state_[j];
    state_[j] = temp;

    t = (state_[i] + state_[j]) % 256;
    return state_[t];
}

void encoder::encode(char const* input, char const* output, bool flag) {
    std::ifstream inputFile(input, std::ios::binary);
    std::ofstream outputFile(output, std::ios::binary);

    std::cout << "Processing...\n";
    KSA();

    char byte;
    while (inputFile.get(byte)) {
        unsigned char keyByte = PRGA();
        byte ^= keyByte;
        outputFile.put(byte);
    }

    inputFile.close();
    outputFile.close();
}

int main() {
    unsigned char key[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    size_t key_size = sizeof(key) / sizeof(key[0]);

    bool flag = true;

    encoder ob(key, key_size);

    unsigned char new_key[] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60};
    size_t new_key_size = sizeof(new_key) / sizeof(new_key[0]);

    ob.mutator(new_key, new_key_size);

    const char* input = "../src/input.txt";
    const char* encrypted = "../src/encrypted.txt";
    const char* decrypted = "../src/decrypted.txt";

    std::cout << "Encrypting...\n";
    ob.encode(input, encrypted, flag);

    std::cout << "Decrypting...\n";
    ob.encode(encrypted, decrypted, flag);

    return 0;
}