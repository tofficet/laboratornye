#include "../../hm4/task.cpp"
#include "twofish.h"
#include <memory>
#include <vector>
#include <cstdint>

using namespace std;

class TwofishWrapper : public IEncryptionAlgorithm {
private:
    unique_ptr<twofish> m_cipher;
    vector<uint8_t> m_key;
    bool m_initialized;

public:
    TwofishWrapper() : m_cipher(make_unique<twofish>()), m_initialized(false) {}

    void encryptBlock(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        const vector<uint8_t>& key
    ) override {
        if (input.size() != getBlockSize()) {
            throw runtime_error("Twofish: invalid block size for encryption");
        }

        if (key.size() != 16 && key.size() != 24 && key.size() != 32) {
            throw runtime_error("Twofish: key must be 16, 24, or 32 bytes");
        }

        if (!m_initialized || m_key != key) {
            m_key = key;
            m_cipher->setup_round_keys(key);
            m_initialized = true;
        }

        output = m_cipher->encrypt_block(input);
    }

    void decryptBlock(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        const vector<uint8_t>& key
    ) override {
        if (input.size() != getBlockSize()) {
            throw runtime_error("Twofish: invalid block size for decryption");
        }

        if (key.size() != 16 && key.size() != 24 && key.size() != 32) {
            throw runtime_error("Twofish: key must be 16, 24, or 32 bytes");
        }

        if (!m_initialized || m_key != key) {
            m_key = key;
            m_cipher->setup_round_keys(key);
            m_initialized = true;
        }

        output = m_cipher->decrypt_block(input);
    }

    size_t getBlockSize() const override {
        return 16;
    }

    size_t getKeySize() const override {
        return 0; // отключаем проверку в CipherContext
    }

    void init(const vector<uint8_t>& key) override {
        if (key.size() != 16 && key.size() != 24 && key.size() != 32) {
            throw runtime_error("Twofish: key must be 16, 24, or 32 bytes");
        }
        m_key = key;
        m_cipher->setup_round_keys(key);
        m_initialized = true;
    }

    void reset() override {
        m_key.clear();
        m_cipher = make_unique<twofish>();
        m_initialized = false;
    }
};