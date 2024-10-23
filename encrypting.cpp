#include <iostream>
#include <random>
#include <fstream>
#include <chrono>
#include <cstdint>

uint64_t OP;
uint64_t KEY;

void initializeKey() {
    std::random_device rd;
    std::mt19937_64 gen(rd());


    std::uniform_int_distribution<uint32_t> dist(1, UINT32_MAX);
    OP = dist(gen);


    std::uniform_int_distribution<uint64_t> distKey(1, UINT64_MAX);
    KEY = distKey(gen) * 1000000000000000000ULL + OP;


    uint64_t xorshift_state = gen();
    KEY ^= (xorshift_state * 1664525 + 1013904223) & 0xFFFFFFFFFFFFFFFFULL;


    KEY = (KEY * 1103515245 + 12345) % UINT64_MAX;


    KEY ^= static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

void XOR(std::string& content, int secretOP, uint64_t pass) { 
    uint8_t operationValue = static_cast<uint8_t>(pass & 0xFF);
    switch (secretOP) {
    case 1:
        for (size_t i = 0; i < content.size(); i++) {
            content[i] ^= operationValue;
        }
        break;
    case 2:
        for (size_t i = 0; i < content.size(); i++) {
            content[i] = static_cast<char>(static_cast<uint8_t>(content[i]) - operationValue);
        }
        break;
    case 3:
        for (size_t i = 0; i < content.size(); i++) {
            content[i] = static_cast<char>(static_cast<uint8_t>(content[i]) * operationValue);
        }
        break;
    case 4:
        if (operationValue != 0) {
            for (size_t i = 0; i < content.size(); i++) {
                content[i] = static_cast<char>(static_cast<uint8_t>(content[i]) / operationValue);
            }
        }
        break;
    default:
        break;
    }
}

bool encrypt(const std::string& fileName, const std::string& content, int contentFromFile) {
    
    std::string encryptedContent;
    int secretOP = KEY % 10;
    if (contentFromFile == 0) {
        encryptedContent = content;
        XOR(encryptedContent, secretOP, KEY);
    }
    else if (contentFromFile == 1) {
        std::ifstream secret(content, std::ios::in | std::ios::binary);
        encryptedContent.assign((std::istreambuf_iterator<char>(secret)), std::istreambuf_iterator<char>());
        XOR(encryptedContent, secretOP, KEY);
    }

    std::ofstream secret(fileName, std::ios::out | std::ios::binary);
    if (!secret) {
        std::cerr << "ERROR 404: Unable to open file " << fileName << "\n";
        return false;
    }

    secret.write(encryptedContent.data(), encryptedContent.size());
    return true;
}

std::string decrypt(const std::string& fileName, uint64_t pass) { 
    int secretOP = pass % 10;

    std::ifstream secret(fileName, std::ios::in | std::ios::binary);
    if (!secret) {
        std::cerr << "ERROR 404: Unable to open file " << fileName << "\n";
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(secret)), std::istreambuf_iterator<char>());

    XOR(content, secretOP, pass);

    std::ofstream decryptedFile("decrypt_" + fileName, std::ios::out | std::ios::binary);
    if (!decryptedFile) {
        std::cerr << "ERROR 404: Unable to create decrypted file.\n";
        return "";
    }

    decryptedFile.write(content.data(), content.size());
    return content;
}
