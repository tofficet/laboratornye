#include <gtest/gtest.h>
#include "rc4.h"

using namespace std;

TEST(RC4Test, EncryptDecrypt) {
    vector<uint8_t> key = {0x01, 0x02, 0x03, 0x04, 0x05};
    vector<uint8_t> plaintext = {'H','e','l','l','o'};
    
    RC4 rc4_enc(key);
    auto ciphertext = rc4_enc.process(plaintext);
    
    RC4 rc4_dec(key);
    auto decrypted = rc4_dec.process(ciphertext);
    
    EXPECT_EQ(decrypted, plaintext);
}

TEST(RC4Test, EmptyData) {
    vector<uint8_t> key = {0x01, 0x02, 0x03};
    vector<uint8_t> plaintext;
    
    RC4 rc4(key);
    auto result = rc4.process(plaintext);
    
    EXPECT_TRUE(result.empty());
}

TEST(RC4Test, SameKeySameStream) {
    vector<uint8_t> key = {0x01, 0x02, 0x03};
    vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04};
    
    RC4 rc4_1(key);
    auto stream1 = rc4_1.process(data);
    
    RC4 rc4_2(key);
    auto stream2 = rc4_2.process(data);
    
    EXPECT_EQ(stream1, stream2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}