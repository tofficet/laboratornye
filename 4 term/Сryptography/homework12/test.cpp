#include "rabin.h"
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cstring>

void printHex(const std::vector<uint8_t>& data) {
    for (auto b : data)
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b;
    std::cout << std::dec;
}

void testMillerRabin() {
    std::cout << "Тест Миллера–Рабина... ";
    assert(Rabin::isPrime(2));
    assert(Rabin::isPrime(3));
    assert(Rabin::isPrime(5));
    assert(Rabin::isPrime(104729));
    assert(!Rabin::isPrime(1));
    assert(!Rabin::isPrime(0));
    assert(!Rabin::isPrime(4));
    assert(!Rabin::isPrime(104730));
    mpz_class bigComposite("1234567890123456789012345678901234567890");
    assert(!Rabin::isPrime(bigComposite, 20));
    std::cout << "ОК" << std::endl;
}

void testKeyGeneration() {
    std::cout << "Тест генерации ключей... ";
    auto [pub, priv] = Rabin::generateKeyPair(128, 20);
    assert(pub.n > 0);
    assert(priv.p > 0);
    assert(priv.q > 0);
    assert(pub.n == priv.p * priv.q);
    assert(priv.p % 4 == 3);
    assert(priv.q % 4 == 3);
    assert(Rabin::isPrime(priv.p, 20));
    assert(Rabin::isPrime(priv.q, 20));
    std::cout << "ОК" << std::endl;
}

void testEncryptDecrypt() {
    std::cout << "Тест шифрования/расшифрования... ";
    auto [pub, priv] = Rabin::generateKeyPair(256, 20);

    {
        std::vector<uint8_t> plain;
        auto cipher = Rabin::encrypt(plain, pub);
        auto decrypted = Rabin::decrypt(cipher, priv);
        assert(decrypted == plain);
    }

    {
        std::string msg = "Hello, Rabin!";
        std::vector<uint8_t> plain(msg.begin(), msg.end());
        auto cipher = Rabin::encrypt(plain, pub);
        auto decrypted = Rabin::decrypt(cipher, priv);
        assert(decrypted == plain);
    }

    {
        std::vector<uint8_t> plain = {0xAB};
        auto cipher = Rabin::encrypt(plain, pub);
        auto decrypted = Rabin::decrypt(cipher, priv);
        assert(decrypted == plain);
    }

    {
        std::vector<uint8_t> plain(500);
        for (size_t i = 0; i < plain.size(); ++i)
            plain[i] = static_cast<uint8_t>(i * 3 + 17);
        auto cipher = Rabin::encrypt(plain, pub);
        auto decrypted = Rabin::decrypt(cipher, priv);
        assert(decrypted == plain);
    }

    {
        std::vector<uint8_t> plain(100, 0x00);
        auto cipher = Rabin::encrypt(plain, pub);
        auto decrypted = Rabin::decrypt(cipher, priv);
        assert(decrypted == plain);
    }

    std::cout << "ОК" << std::endl;
}

void testDecryptionFailures() {
    std::cout << "Тест обнаружения ошибок расшифрования... ";
    auto [pub, priv] = Rabin::generateKeyPair(256, 20);
    std::vector<uint8_t> plain = {'T', 'e', 's', 't'};
    auto cipher = Rabin::encrypt(plain, pub);

    auto broken = cipher;
    broken[0] ^= 0xFF;
    try {
        Rabin::decrypt(broken, priv);
        assert(false && "Ожидалось исключение");
    } catch (const std::runtime_error&) {}

    std::vector<uint8_t> garbage(cipher.size(), 0x00);
    try {
        Rabin::decrypt(garbage, priv);
        assert(false && "Ожидалось исключение");
    } catch (const std::runtime_error&) {}

    std::cout << "ОК" << std::endl;
}

void demo() {
    std::cout << "\n Демонстрация криптосистемы Рабина " << std::endl;

    auto [pub, priv] = Rabin::generateKeyPair(512);
    std::cout << "Открытый ключ n (" << mpz_sizeinbase(pub.n.get_mpz_t(), 2) << " бит): "
              << pub.n << std::endl;

    std::string message = "Криптосистема Рабина с произвольной длиной байтовых последовательностей.";
    std::vector<uint8_t> plain(message.begin(), message.end());

    auto cipher = Rabin::encrypt(plain, pub);
    std::cout << "Исходный текст:  " << message << std::endl;
    std::cout << "Шифротекст (hex): ";
    printHex(cipher);
    std::cout << std::endl;

    auto decrypted = Rabin::decrypt(cipher, priv);
    std::string decMessage(decrypted.begin(), decrypted.end());
    std::cout << "Расшифрованный:  " << decMessage << std::endl;

    if (plain == decrypted)
        std::cout << "Успех: исходное сообщение восстановлено!" << std::endl;
    else
        std::cerr << "Ошибка: сообщения различаются!" << std::endl;
}

int main() {
    testMillerRabin();
    testKeyGeneration();
    testEncryptDecrypt();
    testDecryptionFailures();
    demo();
    return 0;
}

