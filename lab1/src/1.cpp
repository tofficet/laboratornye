#include <fstream>
#include <iostream>
#include <stdexcept>

class encoder final {
   private:
    unsigned char* key_;
    size_t key_size_;
    unsigned char* state_;
    int i, j, t;

    void KSA();
    unsigned char PRGA();

    void allocateStateAndKey();
    void copyStateAndKey(const encoder& other);
    void deallocateResources();

   public:
    encoder(unsigned char const* key, size_t key_size);
    ~encoder();
    void encode(char const* input, char const* output, bool flag);
    void mutator(unsigned char const* key, size_t key_size);
    encoder(const encoder& other);
    encoder& operator=(const encoder& other);
};

void encoder::allocateStateAndKey() {
    try {
        state_ = new unsigned char[256];
        key_ = new unsigned char[key_size_];
    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("Memory allocation failed: " + std::string(e.what()));
    }
}

void encoder::copyStateAndKey(const encoder& other) {
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

void encoder::deallocateResources() {
    delete[] key_;
    delete[] state_;
}

encoder::encoder(unsigned char const* key, size_t key_size) : key_size_(key_size), i(0), j(0) {
    allocateStateAndKey();
    for (size_t index = 0; index < key_size_; ++index) {
        key_[index] = key[index];
    }
}

encoder::encoder(const encoder& other) : key_size_(other.key_size_) {
    allocateStateAndKey();
    copyStateAndKey(other);
}

encoder& encoder::operator=(const encoder& other) {
    if (this == &other) {
        return *this;
    }

    deallocateResources();
    key_size_ = other.key_size_;
    allocateStateAndKey();
    copyStateAndKey(other);

    return *this;
}

encoder::~encoder() { deallocateResources(); }

void encoder::mutator(const unsigned char* new_key, size_t new_key_size) {
    if (new_key_size > 256) {
        throw std::invalid_argument("Key size too large");
    }

    deallocateResources();
    key_size_ = new_key_size;
    allocateStateAndKey();

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
        std::swap(state_[i], state_[j]);
    }
    i = j = 0;
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
    if (!inputFile) throw std::runtime_error("Cannot open input file");

    std::ofstream outputFile(output, std::ios::binary);
    if (!outputFile) throw std::runtime_error("Cannot open output file");

    KSA();

    char byte;
    while (inputFile.get(byte)) {
        outputFile.put(byte ^ PRGA());
    }
}

int main() {
    try {
        unsigned char key[] = {0x01, 0x02, 0x03, 0x04, 0x05};
        encoder ob(key, sizeof(key));

        unsigned char new_key[] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60};
        ob.mutator(new_key, sizeof(new_key));

        ob.encode("../test/input.txt", "../test/encrypted.txt", true);
        ob.encode("../test/encrypted.txt", "../test/decrypted.txt", true);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}