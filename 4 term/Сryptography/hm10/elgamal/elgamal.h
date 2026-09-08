#include <random>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>
#include "../../hm6/task1/miller-rabin_test.h"
#include "../../hm11/primitive_roots.h"

using namespace std;
using namespace boost::multiprecision;

struct el_gamal_public_key {
    cpp_int p;   // простое число
    cpp_int g;   // первообразный корень по модулю p
    cpp_int y;   // y = g^x mod p
};

struct el_gamal_private_key {
    cpp_int x;   // секретный ключ
};

struct el_gamal_ciphertext {
    cpp_int a;   // a = g^k mod p
    cpp_int b;   // b = m * y^k mod p
};

class el_gamal {
private:
    static size_t calculate_block_size(const cpp_int& p) {
        size_t bits = boost::multiprecision::msb(p) + 1;
        return (bits - 1) / 8;
    }

    static cpp_int random_bigint(const cpp_int& min, const cpp_int& max) {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

        cpp_int range = max - min + 1;
        cpp_int result = 0;
        int bits = boost::multiprecision::msb(range) + 1;
        int words = (bits + 63) / 64;

        do {
            result = 0;
            for (int i = 0; i < words; ++i) {
                result <<= 64;
                result |= dist(gen);
            }
            result %= range;
        } while (result < 0);

        return result + min;
    }

    static void write_bigint(ofstream& out, const cpp_int& val) {
        vector<uint8_t> bytes;
        boost::multiprecision::export_bits(val, back_inserter(bytes), 8);
        size_t len = bytes.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(reinterpret_cast<const char*>(bytes.data()), len);
    }

    static cpp_int read_bigint(ifstream& in) {
        size_t len;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        vector<uint8_t> bytes(len);
        in.read(reinterpret_cast<char*>(bytes.data()), len);
        cpp_int val;
        boost::multiprecision::import_bits(val, bytes.begin(), bytes.end());
        return val;
    }

public:
    static void generate_keys(const cpp_int& min_p, const cpp_int& max_p,
                              el_gamal_public_key& pub, el_gamal_private_key& priv) {
        random_device rd;
        mt19937_64 gen(rd());
        MillerRabinTest tester;

        // генерация простого числа p
        cpp_int p;
        while (true) {
            p = random_bigint(min_p, max_p);
            if (p % 2 != 0 && tester.isPrime(static_cast<uint64_t>(p), 0.999))
                break;
        }

        // поиск первообразного корня g
        cpp_int root = primitiveRoot(p);
        if (root == 0) {
            throw runtime_error("No primitive roots found for p");
        }
        cpp_int g = root;

        // генерация секретного ключа x (1 < x < p-1)
        cpp_int x = random_bigint(2, p - 2);

        // вычисление открытого ключа y = g^x mod p
        cpp_int y = powmod(g, x, p);

        pub.p = p;
        pub.g = g;
        pub.y = y;
        priv.x = x;
    }

    static el_gamal_ciphertext encrypt(const cpp_int& message, const el_gamal_public_key& pub) {
        // случайное k (1 < k < p-1)
        cpp_int k = random_bigint(2, pub.p - 2);

        // a = g^k mod p
        cpp_int a = powmod(pub.g, k, pub.p);

        // s = y^k mod p
        cpp_int s = powmod(pub.y, k, pub.p);

        // b = m * s mod p
        cpp_int b = (message % pub.p * s) % pub.p;

        return {a, b};
    }

    static cpp_int decrypt(const el_gamal_ciphertext& cipher,
                           const el_gamal_public_key& pub,
                           const el_gamal_private_key& priv) {
        // s = a^x mod p
        cpp_int s = powmod(cipher.a, priv.x, pub.p);

        // s^{-1} mod p
        cpp_int s_inv = mod_inverse(s, pub.p);

        if (s_inv == 0) {
            throw runtime_error("Modular inverse does not exist");
        }

        // m = b * s^{-1} mod p
        return (cipher.b * s_inv) % pub.p;
    }

    static void encrypt_file(const string& input_path,
                             const string& output_path,
                             const el_gamal_public_key& pub) {
        ifstream in(input_path, ios::binary);
        if (!in) {
            throw runtime_error("Cannot open input file: " + input_path);
        }

        ofstream out(output_path, ios::binary);
        if (!out) {
            throw runtime_error("Cannot open output file: " + output_path);
        }

        vector<uint8_t> buffer((istreambuf_iterator<char>(in)),
                               istreambuf_iterator<char>());
        in.close();

        size_t block_size = calculate_block_size(pub.p);
        if (block_size == 0) block_size = 1;

        // сохраняем размер оригинального файла для восстановления при дешифровании
        size_t total_size = buffer.size();
        out.write(reinterpret_cast<const char*>(&total_size), sizeof(total_size));

        for (size_t i = 0; i < buffer.size(); i += block_size) {
            size_t chunk_size = min(block_size, buffer.size() - i);

            cpp_int m = 0;
            for (size_t j = 0; j < chunk_size; ++j)
                m = (m << 8) | buffer[i + j];

            el_gamal_ciphertext cipher = encrypt(m, pub);

            // записываем a и b в файл
            write_bigint(out, cipher.a);
            write_bigint(out, cipher.b);
        }
    }

    static void decrypt_file(const string& input_path,
                             const string& output_path,
                             const el_gamal_public_key& pub,
                             const el_gamal_private_key& priv) {
        ifstream in(input_path, ios::binary);
        if (!in) {
            throw runtime_error("Cannot open input file: " + input_path);
        }

        ofstream out(output_path, ios::binary);
        if (!out) {
            throw runtime_error("Cannot open output file: " + output_path);
        }

        // читаем размер оригинального файла
        size_t original_size;
        in.read(reinterpret_cast<char*>(&original_size), sizeof(original_size));

        size_t block_size = calculate_block_size(pub.p);
        if (block_size == 0) 
            block_size = 1;

        size_t bytes_written = 0;

        while (bytes_written < original_size) {
            // читаем a и b из файла
            cpp_int a = read_bigint(in);
            cpp_int b = read_bigint(in);

            el_gamal_ciphertext cipher{a, b};
            cpp_int m = decrypt(cipher, pub, priv);

            size_t chunk_size = min(block_size, original_size - bytes_written);

            // извлекаем байты из m
            vector<uint8_t> decrypted_bytes(chunk_size);
            for (size_t j = 0; j < chunk_size; ++j) {
                decrypted_bytes[chunk_size - 1 - j] = static_cast<uint8_t>(m & 0xFF);
                m >>= 8;
            }

            out.write(reinterpret_cast<const char*>(decrypted_bytes.data()), chunk_size);
            bytes_written += chunk_size;
        }
    }
};