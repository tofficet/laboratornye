#include "ElGamal.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <chrono>

void testKeyGeneration() {
    std::cout << "Тест генерации ключей (1024 бит, 25 раундов)... " << std::flush;
    try {
        auto start = std::chrono::high_resolution_clock::now();
        
        auto [pub, priv] = ElGamal::generateKeyPair(1024, 25);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        
        assert(pub.p > 0);
        assert(pub.g > 0);
        assert(pub.y > 0);
        assert(priv.x > 0);
        assert(priv.p == pub.p);
        assert(priv.g == pub.g);
        
        mpz_class y2;
        mpz_powm(y2.get_mpz_t(), pub.g.get_mpz_t(), priv.x.get_mpz_t(), pub.p.get_mpz_t());
        assert(pub.y == y2);
        
        size_t bitSize = mpz_sizeinbase(pub.p.get_mpz_t(), 2);
        std::cout << "OK (размер: " << bitSize << " бит, время: " << duration.count() << " сек)" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "FAILED: " << e.what() << std::endl;
        throw;
    }
}

void testSignVerify() {
    std::cout << "Тест подписи и проверки (1024 бит, 25 раундов)... " << std::flush;
    try {
        auto start = std::chrono::high_resolution_clock::now();
        
        auto [pub, priv] = ElGamal::generateKeyPair(1024, 25);
        
        std::string msg = "Hello, ElGamal!";
        std::vector<uint8_t> plain(msg.begin(), msg.end());
        auto sig = ElGamal::sign(plain, priv);
        assert(ElGamal::verify(plain, sig, pub));
        
        std::vector<uint8_t> empty;
        sig = ElGamal::sign(empty, priv);
        assert(ElGamal::verify(empty, sig, pub));
        
        std::string longMsg(10000, 'A');
        std::vector<uint8_t> longVec(longMsg.begin(), longMsg.end());
        sig = ElGamal::sign(longVec, priv);
        assert(ElGamal::verify(longVec, sig, pub));
        
        std::vector<uint8_t> bad(msg.begin(), msg.end());
        bad[0] ^= 0xFF;
        assert(!ElGamal::verify(bad, sig, pub));
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "OK (время: " << duration.count() << " мс)" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "FAILED: " << e.what() << std::endl;
        throw;
    }
}

void testSignatureSerialization() {
    std::cout << "Тест сериализации подписи (1024 бит, 25 раундов)... " << std::flush;
    try {
        auto [pub, priv] = ElGamal::generateKeyPair(1024, 25);
        std::vector<uint8_t> msg = {'t','e','s','t'};
        auto sig = ElGamal::sign(msg, priv);
        
        size_t pLen = (mpz_sizeinbase(pub.p.get_mpz_t(), 2) + 7) / 8;
        
        auto bytes = ElGamal::signatureToBytes(sig, pLen);
        auto restored = ElGamal::signatureFromBytes(bytes, pLen);
        assert(restored.r == sig.r && restored.s == sig.s);
        assert(ElGamal::verify(msg, restored, pub));
        
        assert(bytes.size() == pLen * 2);
        
        std::cout << "OK" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "FAILED: " << e.what() << std::endl;
        throw;
    }
}

void demo() {
    std::cout << "\nДемонстрация подписи Эль-Гамаля (1024 бит)\n";
    try {
        auto start = std::chrono::high_resolution_clock::now();
        
        auto [pub, priv] = ElGamal::generateKeyPair(1024, 25);
        
        auto genEnd = std::chrono::high_resolution_clock::now();
        
        std::string message = "Это сообщение для подписи Эль-Гамаля с использованием 1024-битного ключа.";
        std::vector<uint8_t> msgVec(message.begin(), message.end());
        
        auto signStart = std::chrono::high_resolution_clock::now();
        auto signature = ElGamal::sign(msgVec, priv);
        auto signEnd = std::chrono::high_resolution_clock::now();
        
        std::cout << "\nПодпись:\n";
        std::cout << " r = " << signature.r << "\n";
        std::cout << " s = " << signature.s << "\n";
        
        bool valid = ElGamal::verify(msgVec, signature, pub);
        std::cout << "\nПроверка подписи: " << (valid ? "ВЕРНА" : "НЕВЕРНА") << "\n";
        
        std::string badMsg = message + " (изменено)";
        std::vector<uint8_t> badVec(badMsg.begin(), badMsg.end());
        bool badValid = ElGamal::verify(badVec, signature, pub);
        std::cout << "Проверка изменённого сообщения: " << (badValid ? "ВЕРНА" : "НЕВЕРНА") << "\n";
        
        auto signDuration = std::chrono::duration_cast<std::chrono::milliseconds>(signEnd - signStart);
        auto genDuration = std::chrono::duration_cast<std::chrono::seconds>(genEnd - start);
        
        std::cout << "\nВремя генерации ключей: " << genDuration.count() << " сек\n";
        std::cout << "Время создания подписи: " << signDuration.count() << " мс\n";
        
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
        throw;
    }
}

int main() {
    std::cout << "Тестирование схемы подписи Эль-Гамаля\n";
    std::cout << "Параметры: 1024 бита, 25 раундов\n\n";
    
    try {
        testKeyGeneration();
        testSignVerify();
        testSignatureSerialization();
        demo();
        
        std::cout << "\nВсе тесты успешно завершены\n";
    } catch (const std::exception& e) {
        std::cout << "\nТесты провалены: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}