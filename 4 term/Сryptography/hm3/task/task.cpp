#include <cstdint>
#include <future>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

enum class ModeType {
    ECB,
    CBC,
    PCBC
};

enum class PaddingType {
    Zeros, 
    ANSI_X923
};


class IEncryptionAlgorithm {
public:
    virtual ~IEncryptionAlgorithm() = default;
    
    virtual void encryptBlock(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        const vector<uint8_t>& key
    ) = 0;

    virtual void decryptBlock(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        const vector<uint8_t>& key
    ) = 0;

    virtual size_t getBlockSize() const = 0;
    virtual size_t getKeySize() const = 0;

    virtual void init(const vector<uint8_t>& key) {}
    virtual void reset() {}
};


class IPaddingMode {
public:
    virtual ~IPaddingMode() = default;

    virtual vector<uint8_t> addPadding(
        const vector<uint8_t>& data,
        size_t blockSize
    ) = 0;

    virtual vector<uint8_t> removePadding(
        const vector<uint8_t>& data,
        size_t blockSize
    ) = 0;

    virtual PaddingType getType() const = 0;
};

class ZerosPadding : public IPaddingMode {
public:
    vector<uint8_t> addPadding(const vector<uint8_t>& data, size_t blockSize) override {
        vector<uint8_t> result = data;
        
        size_t remainder = data.size() % blockSize;
        size_t need = (remainder == 0) ? 0 : blockSize - remainder;
        
        result.resize(data.size() + need, 0);
        return result;
    }

    vector<uint8_t> removePadding(const vector<uint8_t>& data, size_t blockSize) override {
        size_t i = data.size();
        while (i > 0 && data[i-1] == 0)
            i--;
        
        return vector<uint8_t>(data.begin(), data.begin() + i);
    }

    PaddingType getType() const override {
        return PaddingType::Zeros;
    }
};

class ANSIX923Padding : public IPaddingMode {
public:
    vector<uint8_t> addPadding(const vector<uint8_t>& data, size_t blockSize) override {
        vector<uint8_t> result = data;
        
        size_t remainder = data.size() % blockSize;
        size_t need = (remainder == 0) ? blockSize : blockSize - remainder;
        
        result.resize(data.size() + need, 0);
        result.back() = static_cast<uint8_t>(need);
        
        return result;
    }

    vector<uint8_t> removePadding(const vector<uint8_t>& data, size_t blockSize) override {
        if (data.empty()) return data;
        
        uint8_t padSize = data.back();
        
        if (padSize == 0 || padSize > blockSize || padSize > data.size())
            throw runtime_error("Invalid ANSI X.923 padding");
        
        return vector<uint8_t>(data.begin(), data.end() - padSize);
    }

    PaddingType getType() const override {
        return PaddingType::ANSI_X923;
    }
};


class XORAlgorithm : public IEncryptionAlgorithm {
private:
    size_t m_blockSize;
    size_t m_keySize;
    
public:
    XORAlgorithm(size_t blockSize = 16, size_t keySize = 16) 
        : m_blockSize(blockSize), m_keySize(keySize) {}
    
    void encryptBlock(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        const vector<uint8_t>& key
    ) override {
        // Проверка размера
        if (input.size() != m_blockSize) {
            throw runtime_error("Invalid block size for XOR");
        }
        
        output.resize(m_blockSize);
        
        // XOR с ключом
        for (size_t i = 0; i < m_blockSize; i++) {
            output[i] = input[i] ^ key[i % key.size()];
        }
    }
    
    void decryptBlock(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        const vector<uint8_t>& key
    ) override {
        // XOR симметричен
        encryptBlock(input, output, key);
    }
    
    size_t getBlockSize() const override {
        return m_blockSize;
    }
    
    size_t getKeySize() const override {
        return m_keySize;
    }
};


class CipherException : public exception {
private:
    string m_message;
public:
    explicit CipherException(const string& msg) : m_message(msg) {}
    const char* what() const noexcept override {
        return m_message.c_str();
    }
};

class InvalidKeyException : public CipherException {
public:
    explicit InvalidKeyException(const string& msg) : CipherException(msg) {}
};

class InvalidDataException : public CipherException {
public:
    explicit InvalidDataException(const string& msg) : CipherException(msg) {}
};

class FileOperationException : public CipherException {
public:
    explicit FileOperationException(const string& msg) : CipherException(msg) {}
};


class IEncryptionMode {
public:
    virtual ~IEncryptionMode() = default;
    
    virtual void encrypt(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        IEncryptionAlgorithm* algorithm,
        const vector<uint8_t>& key,
        size_t numThreads = 1
    ) = 0;
    
    virtual void decrypt(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        IEncryptionAlgorithm* algorithm,
        const vector<uint8_t>& key,
        size_t numThreads = 1
    ) = 0;
    
    virtual void setIV(const vector<uint8_t>& iv) = 0;
    
    virtual vector<uint8_t> getIV() const = 0;
    
    virtual ModeType getType() const = 0;
};

class ECBMode : public IEncryptionMode {
private:
    vector<uint8_t> m_iv;

public:
    ECBMode(const vector<uint8_t>& iv = {}) : m_iv(iv) {}

    void encrypt(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        IEncryptionAlgorithm* algorithm,
        const vector<uint8_t>& key,
        size_t numThreads = 1
    ) override {
        size_t blockSize = algorithm->getBlockSize();

        if (input.size() % blockSize != 0)
            throw InvalidDataException("ECB mode: input size must be multiple of block size");

        output.resize(input.size());
        size_t numBlocks = input.size() / blockSize;

        if (numThreads <= 1 || numBlocks < numThreads) {
            for (size_t i = 0; i < numBlocks; i++) {
                vector<uint8_t> inBlock(
                    input.begin() + i * blockSize,
                    input.begin() + (i + 1) * blockSize
                );

                vector<uint8_t> outBlock(blockSize);

                algorithm->encryptBlock(inBlock, outBlock, key);

                for (size_t j = 0; j < blockSize; j++)
                    output[i * blockSize + j] = outBlock[j];
            }
        } else {
            vector<future<void>> futures;
            size_t blocksPerThread = (numBlocks + numThreads - 1) / numThreads;
            
            for (size_t t = 0; t < numThreads; t++) {
                size_t start = t * blocksPerThread;
                size_t end = min(start + blocksPerThread, numBlocks);
                
                futures.push_back(async(launch::async, [&, start, end]() {
                    for (size_t i = start; i < end; i++) {
                        vector<uint8_t> inBlock(
                            input.begin() + i * blockSize,
                            input.begin() + (i + 1) * blockSize
                        );

                        vector<uint8_t> outBlock(blockSize);

                        algorithm->encryptBlock(inBlock, outBlock, key);

                        for (size_t j = 0; j < blockSize; j++)
                            output[i * blockSize + j] = outBlock[j];   
                    }
                }));
            }

            for (auto& f : futures) 
                f.get();
        }
    }

    void decrypt(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        IEncryptionAlgorithm* algorithm,
        const vector<uint8_t>& key,
        size_t numThreads = 1
    ) override {
        size_t blockSize = algorithm->getBlockSize();

        if (input.size() % blockSize != 0)
            throw InvalidDataException("ECB mode: input size must be multiple of block size");

        output.resize(input.size());
        size_t numBlocks = input.size() / blockSize;

        if (numThreads <= 1 || numBlocks < numThreads) {
            for (size_t i = 0; i < numBlocks; i++) {
                vector<uint8_t> inBlock(
                    input.begin() + i * blockSize,
                    input.begin() + (i + 1) * blockSize
                );
                
                vector<uint8_t> outBlock(blockSize);
                
                algorithm->decryptBlock(inBlock, outBlock, key);
                
                for (size_t j = 0; j < blockSize; j++)
                    output[i * blockSize + j] = outBlock[j];
            }
        } else {
            // Многопоточная обработка
            vector<future<void>> futures;
            size_t blocksPerThread = (numBlocks + numThreads - 1) / numThreads;
            
            for (size_t t = 0; t < numThreads; t++) {
                size_t startBlock = t * blocksPerThread;
                size_t endBlock = min(startBlock + blocksPerThread, numBlocks);
                
                futures.push_back(async(launch::async, 
                    [&input, &output, algorithm, &key, blockSize, startBlock, endBlock]() {
                        for (size_t i = startBlock; i < endBlock; i++) {
                            vector<uint8_t> inBlock(
                                input.begin() + i * blockSize,
                                input.begin() + (i + 1) * blockSize
                            );
                            vector<uint8_t> outBlock(blockSize);
                            algorithm->decryptBlock(inBlock, outBlock, key);
                            for (size_t j = 0; j < blockSize; j++)
                                output[i * blockSize + j] = outBlock[j];
                        }
                    }
                ));
            }
            
            for (auto& f : futures) {
                f.get();
            }
        }
    }

    void setIV(const vector<uint8_t>& iv) override { 
        m_iv = iv; 
    }

    vector<uint8_t> getIV() const override { 
        return m_iv; 
    }

    ModeType getType() const override { 
        return ModeType::ECB; 
    }
};

// РЕЖИМ CBC (Cipher Block Chaining)
class CBCMode : public IEncryptionMode {
private:
    vector<uint8_t> m_iv;  // вектор инициализации
    
public:
    CBCMode(const vector<uint8_t>& iv) : m_iv(iv) {
        if (iv.empty())
            throw InvalidDataException("CBC mode requires IV");
    }
    
    void encrypt(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        IEncryptionAlgorithm* algorithm,
        const vector<uint8_t>& key,
        size_t numThreads = 1
    ) override {
        size_t blockSize = algorithm->getBlockSize();
        
        // Проверяем размер IV
        if (m_iv.size() != blockSize) {
            throw InvalidDataException("CBC mode: IV size must match block size");
        }
        
        // Проверяем входные данные
        if (input.size() % blockSize != 0) {
            throw InvalidDataException("CBC mode: input size must be multiple of block size");
        }
        
        output.resize(input.size());
        size_t numBlocks = input.size() / blockSize;
        
        // Начинаем с вектора инициализации
        vector<uint8_t> previous = m_iv;
        
        for (size_t i = 0; i < numBlocks; i++) {
            // 1. Берем текущий блок исходных данных
            vector<uint8_t> plainBlock(
                input.begin() + i * blockSize,
                input.begin() + (i + 1) * blockSize
            );
            
            // 2. XOR с предыдущим зашифрованным блоком (или IV для первого)
            vector<uint8_t> xorBlock(blockSize);
            for (size_t j = 0; j < blockSize; j++) {
                xorBlock[j] = plainBlock[j] ^ previous[j];
            }
            
            // 3. Шифруем результат XOR
            vector<uint8_t> cipherBlock(blockSize);
            algorithm->encryptBlock(xorBlock, cipherBlock, key);
            
            // 4. Сохраняем результат
            for (size_t j = 0; j < blockSize; j++) {
                output[i * blockSize + j] = cipherBlock[j];
            }
            
            // 5. Этот блок становится предыдущим для следующего
            previous = cipherBlock;
        }
    }
    
    void decrypt(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        IEncryptionAlgorithm* algorithm,
        const vector<uint8_t>& key,
        size_t numThreads = 1
    ) override {
        size_t blockSize = algorithm->getBlockSize();
        
        if (m_iv.size() != blockSize) {
            throw InvalidDataException("CBC mode: IV size must match block size");
        }
        
        if (input.size() % blockSize != 0) {
            throw InvalidDataException("CBC mode: input size must be multiple of block size");
        }
        
        output.resize(input.size());
        size_t numBlocks = input.size() / blockSize;
        
        
        if (numThreads <= 1 || numBlocks < numThreads) {
            vector<uint8_t> previous = m_iv;
            
            for (size_t i = 0; i < numBlocks; i++) {
                // 1. Берем текущий зашифрованный блок
                vector<uint8_t> cipherBlock(
                    input.begin() + i * blockSize,
                    input.begin() + (i + 1) * blockSize
                );
                
                // 2. Расшифровываем его
                vector<uint8_t> decryptedBlock(blockSize);
                algorithm->decryptBlock(cipherBlock, decryptedBlock, key);
                
                // 3. XOR с предыдущим зашифрованным блоком (или IV)
                for (size_t j = 0; j < blockSize; j++) {
                    output[i * blockSize + j] = decryptedBlock[j] ^ previous[j];
                }
                
                // 4. Этот зашифрованный блок становится предыдущим для следующего
                previous = cipherBlock;
            }
        } else {
            // Многопоточное дешифрование CBC
            vector<future<void>> futures;
            size_t blocksPerThread = (numBlocks + numThreads - 1) / numThreads;
            
            // Нужен общий доступ к previous, поэтому используем вектор для хранения cipher блоков
            vector<vector<uint8_t>> cipherBlocks(numBlocks);
            for (size_t i = 0; i < numBlocks; i++) {
                cipherBlocks[i].assign(
                    input.begin() + i * blockSize,
                    input.begin() + (i + 1) * blockSize
                );
            }
            
            for (size_t t = 0; t < numThreads; t++) {
                size_t start = t * blocksPerThread;
                size_t end = min(start + blocksPerThread, numBlocks);
                
                futures.push_back(async(launch::async, 
                    [&output, algorithm, &key, &cipherBlocks, blockSize, start, end, this]() {
                        for (size_t i = start; i < end; i++) {
                            vector<uint8_t> decrypted(blockSize);
                            algorithm->decryptBlock(cipherBlocks[i], decrypted, key);
                            
                            // Получаем предыдущий блок (IV для первого)
                            vector<uint8_t> prev;
                            if (i == 0) {
                                prev = m_iv;
                            } else {
                                prev = cipherBlocks[i-1];
                            }
                            
                            for (size_t j = 0; j < blockSize; j++) {
                                output[i * blockSize + j] = decrypted[j] ^ prev[j];
                            }
                        }
                    }
                ));
            }
            
            for (auto& f : futures) 
                f.get();
        }
    }
    
    void setIV(const vector<uint8_t>& iv) override {
        m_iv = iv;
    }
    
    vector<uint8_t> getIV() const override {
        return m_iv;
    }
    
    ModeType getType() const override {
        return ModeType::CBC;
    }
};

class PCBCMode : public IEncryptionMode {
private:
    vector<uint8_t> m_iv;  // вектор инициализации
    
public:
    PCBCMode(const vector<uint8_t>& iv) : m_iv(iv) {
        if (iv.empty())
            throw InvalidDataException("PCBC mode requires IV");
    }
    
    void encrypt(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        IEncryptionAlgorithm* algorithm,
        const vector<uint8_t>& key,
        size_t numThreads = 1
    ) override {
        size_t blockSize = algorithm->getBlockSize();
        
        // Проверяем размер IV
        if (m_iv.size() != blockSize) {
            throw InvalidDataException("PCBC mode: IV size must match block size");
        }
        
        // Проверяем входные данные
        if (input.size() % blockSize != 0) {
            throw InvalidDataException("PCBC mode: input size must be multiple of block size");
        }
        
        output.resize(input.size());
        size_t numBlocks = input.size() / blockSize;
        
        // Для PCBC нужно хранить И предыдущий зашифрованный, И предыдущий исходный
        vector<uint8_t> prevCipher = m_iv;      // предыдущий зашифрованный (для первого - IV)
        vector<uint8_t> prevPlain(blockSize, 0); // предыдущий исходный (для первого - нули)
        
        for (size_t i = 0; i < numBlocks; i++) {
            // 1. Берем текущий блок исходных данных
            vector<uint8_t> plainBlock(
                input.begin() + i * blockSize,
                input.begin() + (i + 1) * blockSize
            );
            
            // 2. XOR с (prevCipher XOR prevPlain)
            // Это ключевое отличие PCBC от CBC!
            vector<uint8_t> xorBlock(blockSize);
            for (size_t j = 0; j < blockSize; j++) {
                xorBlock[j] = plainBlock[j] ^ prevCipher[j] ^ prevPlain[j];
            }
            
            // 3. Шифруем результат XOR
            vector<uint8_t> cipherBlock(blockSize);
            algorithm->encryptBlock(xorBlock, cipherBlock, key);
            
            // 4. Сохраняем результат
            for (size_t j = 0; j < blockSize; j++) {
                output[i * blockSize + j] = cipherBlock[j];
            }
            
            // 5. Сохраняем для следующего блока
            prevCipher = cipherBlock;           // запоминаем зашифрованный
            prevPlain = plainBlock;              // запоминаем исходный
        }
    }
    
    void decrypt(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        IEncryptionAlgorithm* algorithm,
        const vector<uint8_t>& key,
        size_t numThreads = 1
    ) override {
        size_t blockSize = algorithm->getBlockSize();
        
        if (m_iv.size() != blockSize) {
            throw InvalidDataException("PCBC mode: IV size must match block size");
        }
        
        if (input.size() % blockSize != 0) {
            throw InvalidDataException("PCBC mode: input size must be multiple of block size");
        }
        
        output.resize(input.size());
        size_t numBlocks = input.size() / blockSize;
        
        vector<uint8_t> prevCipher = m_iv;
        vector<uint8_t> prevPlain(blockSize, 0);
        
        for (size_t i = 0; i < numBlocks; i++) {
            // 1. Берем текущий зашифрованный блок
            vector<uint8_t> cipherBlock(
                input.begin() + i * blockSize,
                input.begin() + (i + 1) * blockSize
            );
            
            // 2. Расшифровываем его
            vector<uint8_t> decryptedBlock(blockSize);
            algorithm->decryptBlock(cipherBlock, decryptedBlock, key);
            
            // 3. XOR с (prevCipher XOR prevPlain)
            vector<uint8_t> plainBlock(blockSize);
            for (size_t j = 0; j < blockSize; j++) {
                plainBlock[j] = decryptedBlock[j] ^ prevCipher[j] ^ prevPlain[j];
            }
            
            // 4. Сохраняем результат
            for (size_t j = 0; j < blockSize; j++) {
                output[i * blockSize + j] = plainBlock[j];
            }
            
            // 5. Сохраняем для следующего блока
            prevCipher = cipherBlock;
            prevPlain = plainBlock;
        }
    }
    
    void setIV(const vector<uint8_t>& iv) override {
        m_iv = iv;
    }
    
    vector<uint8_t> getIV() const override {
        return m_iv;
    }
    
    ModeType getType() const override {
        return ModeType::PCBC;
    }
};

class CipherContext {
private:
    unique_ptr<IEncryptionAlgorithm> m_algorithm;  // алгоритм (XOR, DES...)
    unique_ptr<IEncryptionMode> m_mode;            // режим (ECB, CBC, PCBC)
    unique_ptr<IPaddingMode> m_padding;            // паддинг (Zeros, ANSI...)
    
public:
    // Конструктор
    CipherContext(
        unique_ptr<IEncryptionAlgorithm> algorithm,
        ModeType mode,
        PaddingType padding,
        const vector<uint8_t>& iv = {},
        const vector<vector<uint8_t>>& modeParams = {}
    ) : m_algorithm(std::move(algorithm)) {
        
        // Создаем нужный режим
        switch (mode) {
            case ModeType::ECB:
                m_mode = make_unique<ECBMode>(iv);
                break;
            case ModeType::CBC:
                m_mode = make_unique<CBCMode>(iv);
                break;
            case ModeType::PCBC:
                m_mode = make_unique<PCBCMode>(iv);
                break;
            default:
                throw CipherException("Unknown mode");
        }
        
        // Создаем нужный паддинг
        switch (padding) {
            case PaddingType::Zeros:
                m_padding = make_unique<ZerosPadding>();
                break;
            case PaddingType::ANSI_X923:
                m_padding = make_unique<ANSIX923Padding>();
                break;
            default:
                throw CipherException("Unknown padding");
        }
    }
    
    // Шифрование данных в памяти
    void encrypt(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        const vector<uint8_t>& key,
        size_t numThreads = 1
    ) {
        // Проверяем ключ
        if (key.size() != m_algorithm->getKeySize()) {
            throw InvalidKeyException(
                "Key size mismatch. Expected: " + to_string(m_algorithm->getKeySize()) +
                ", got: " + to_string(key.size())
            );
        }
        
        // 1. Добавляем паддинг
        vector<uint8_t> padded = m_padding->addPadding(input, m_algorithm->getBlockSize());
        
        // 2. Шифруем
        m_mode->encrypt(padded, output, m_algorithm.get(), key, numThreads);
    }
    
    // Дешифрование данных в памяти
    void decrypt(
        const vector<uint8_t>& input,
        vector<uint8_t>& output,
        const vector<uint8_t>& key,
        size_t numThreads = 1
    ) {
        // Проверяем ключ
        if (key.size() != m_algorithm->getKeySize()) {
            throw InvalidKeyException(
                "Key size mismatch. Expected: " + to_string(m_algorithm->getKeySize()) +
                ", got: " + to_string(key.size())
            );
        }
        
        // 1. Дешифруем
        vector<uint8_t> decrypted;
        m_mode->decrypt(input, decrypted, m_algorithm.get(), key, numThreads);
        
        // 2. Убираем паддинг
        output = m_padding->removePadding(decrypted, m_algorithm->getBlockSize());
    }

    // Асинхронное шифрование файла
    future<void> encryptFileAsync(
        const string& inputPath,
        const string& outputPath,
        const vector<uint8_t>& key,
        size_t numThreads = 1
    ) {
        return async(launch::async, [this, inputPath, outputPath, key, numThreads]() {
            try {
                // Открываем входной файл
                ifstream inFile(inputPath, ios::binary);
                if (!inFile) {
                    throw FileOperationException("Cannot open input file: " + inputPath);
                }
                
                // Читаем весь файл
                vector<uint8_t> data(
                    (istreambuf_iterator<char>(inFile)),
                    istreambuf_iterator<char>()
                );
                inFile.close();
                
                // Шифруем
                vector<uint8_t> encrypted;
                encrypt(data, encrypted, key, numThreads);
                
                // Записываем результат
                ofstream outFile(outputPath, ios::binary);
                if (!outFile) {
                    throw FileOperationException("Cannot create output file: " + outputPath);
                }
                outFile.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
                outFile.close();
                
            } catch (...) {
                // Пробрасываем исключение в future
                throw;
            }
        });
    }
    
    // Асинхронное дешифрование файла
    future<void> decryptFileAsync(
        const string& inputPath,
        const string& outputPath,
        const vector<uint8_t>& key,
        size_t numThreads = 1
    ) {
        return async(launch::async, [this, inputPath, outputPath, key, numThreads]() {
            try {
                // Открываем входной файл
                ifstream inFile(inputPath, ios::binary);
                if (!inFile) {
                    throw FileOperationException("Cannot open input file: " + inputPath);
                }
                
                // Читаем весь файл
                vector<uint8_t> data(
                    (istreambuf_iterator<char>(inFile)),
                    istreambuf_iterator<char>()
                );
                inFile.close();
                
                // Дешифруем
                vector<uint8_t> decrypted;
                decrypt(data, decrypted, key, numThreads);
                
                // Записываем результат
                ofstream outFile(outputPath, ios::binary);
                if (!outFile) {
                    throw FileOperationException("Cannot create output file: " + outputPath);
                }
                outFile.write(reinterpret_cast<const char*>(decrypted.data()), decrypted.size());
                outFile.close();
                
            } catch (...) {
                throw;
            }
        });
    }
    
    // Получить тип режима
    ModeType getModeType() const {
        return m_mode->getType();
    }
    
    // Получить тип паддинга
    PaddingType getPaddingType() const {
        return m_padding->getType();
    }
    
    // Получить IV
    vector<uint8_t> getIV() const {
        return m_mode->getIV();
    }
};