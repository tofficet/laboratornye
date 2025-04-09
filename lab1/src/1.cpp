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
    encoder(const encoder& other);
    encoder& operator=(const encoder& other);
};

encoder::encoder(unsigned char const* key, size_t key_size) {
 try{
    state_ = new unsigned char[256];
    key_ = new unsigned char[key_size_];
 } catch (const std::bad_alloc& e){
    std::cerr<<"Error: "<< e.what()<<std::endl;
    throw;
 }

    key_size_ = key_size;
    i = 0;
    j = 0;

    for (size_t index = 0; index < key_size_; ++index) {
        key_[index] = key[index];
    }
}

encoder::encoder(const encoder& other) {
    key_size_ = other.key_size_;
    try{
    key_ = new unsigned char[key_size_];
    state_ = new unsigned char[256];
    }catch(const std::bad_alloc& e){
        std::cerr<<"Error: "<< e.what()<<std::endl;
        throw;
    }

    for (size_t index = 0; index < key_size_; ++index) {
        key_[index] = other.key_[index];
    }
    for (int index = 0; index < 256; ++index) {
        state_[index] = other.state_[index];
    }
    i = other.i;
    j = other.j;
    t = other.t;
}

encoder& encoder::operator=(const encoder& other) {
    if (this == &other) {
        return *this;
    }

    delete[] key_;
    delete[] state_;

    key_size_ = other.key_size_;
    try{
    key_ = new unsigned char[key_size_];
    state_ = new unsigned char[256];
    }catch(const std::bad_alloc& e){
        std::cerr<<"Error: "<< e.what()<<std::endl;
        throw;
    }

    for (size_t index = 0; index < key_size_; ++index) {
        key_[index] = other.key_[index];
    }
    for (int index = 0; index < 256; ++index) {
        state_[index] = other.state_[index];
    }
    i = other.i;
    j = other.j;
    t = other.t;

    return *this;
}

encoder::~encoder() {
    delete[] key_;
    delete[] state_;
}

void encoder::mutator(const unsigned char* new_key, size_t new_key_size) {
    if (new_key_size > 256) {
        return;
    }

    delete[] key_;
    key_size_ = new_key_size;

    try{
    key_ = new unsigned char[key_size_];
    }catch(const std::bad_alloc& e){
        std::cerr<<"Error: "<< e.what()<<std::endl;
        throw;
    }
    

    for (size_t i = 0; i < key_size_; ++i) {
        key_[i] = new_key[i];
    }

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

    const char* input = "../test/input.txt";
    const char* encrypted = "../test/encrypted.txt";
    const char* decrypted = "../test/decrypted.txt";

    ob.encode(input, encrypted, flag);

    ob.encode(encrypted, decrypted, flag);

    return 0;
}