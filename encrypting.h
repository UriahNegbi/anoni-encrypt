#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <string>


void initializeKey();


void XOR(std::string& content, int secretOP, uint64_t pass);


bool encrypt(const std::string& fileName, const std::string& content, int contentFromFile);


std::string decrypt(const std::string& fileName, uint64_t pass);


extern uint64_t KEY;
int contentFromFile;
#endif
