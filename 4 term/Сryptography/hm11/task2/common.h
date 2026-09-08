#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

const cpp_int P = cpp_int("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
const cpp_int G = 2;
const int PORT = 8080;
const char* SERVER_IP = "127.0.0.1";

cpp_int powmod(cpp_int base, cpp_int exp, cpp_int mod) {
    cpp_int result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) 
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

void send_bigint(int socket_fd, const cpp_int& value) {
    string s = value.str();
    uint32_t len = s.length();
    send(socket_fd, &len, sizeof(len), 0);
    send(socket_fd, s.c_str(), len, 0);
}

cpp_int receive_bigint(int socket_fd) {
    uint32_t len;
    recv(socket_fd, &len, sizeof(len), 0);
    vector<char> buffer(len + 1, 0);
    recv(socket_fd, buffer.data(), len, 0);
    return cpp_int(buffer.data());
}

vector<uint8_t> deriveKey(const cpp_int& secret, size_t keySize) {
    vector<uint8_t> key(keySize, 0);
    string s = secret.str();
    for (size_t i = 0; i < keySize && i < s.length(); i++) {
        key[i] = s[i];
    }
    return key;
}