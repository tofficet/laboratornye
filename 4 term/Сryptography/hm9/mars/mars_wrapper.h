#include "../../hm4/task.cpp"
#include "mars.h"
#include <memory>
#include <vector>
#include <cstdint>

using namespace std;

class MarsWrapper : public IEncryptionAlgorithm {
private:
    unique_ptr<mars> m_cipher;
    vector<uint8_t> m_key;
    bool m_initialized;

public:
    MarsWrapper() : m_cipher(make_unique<mars>()), m_initialized(false) {}

    void encryptBlock(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        const vector<uint8_t>& key
    ) override {
        if (input.size() != getBlockSize()) {
            throw runtime_error("MARS: invalid block size for encryption");
        }

        if (key.size() < 16 || key.size() > 56) {
            throw runtime_error("MARS: key must be between 16 and 56 bytes");
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
            throw runtime_error("MARS: invalid block size for decryption");
        }

        if (key.size() < 16 || key.size() > 56) {
            throw runtime_error("MARS: key must be between 16 and 56 bytes");
        }

        if (!m_initialized || m_key != key) {
            m_key = key;
            m_cipher->setup_round_keys(key);
            m_initialized = true;
        }

        output = m_cipher->decrypt_block(input);
    }

    size_t getBlockSize() const override { return 16; }

    size_t getKeySize() const override { return 0; }

    void init(const vector<uint8_t>& key) override {
        if (key.size() < 16 || key.size() > 56) {
            throw runtime_error("MARS: key must be between 16 and 56 bytes");
        }
        m_key = key;
        m_cipher->setup_round_keys(key);
        m_initialized = true;
    }

    void reset() override {
        m_key.clear();
        m_cipher = make_unique<mars>();
        m_initialized = false;
    }
};