#include <iostream>
#include <string>
#include "encrypting.h" 
#include <cstdint>

int main() {
    initializeKey();

    std::string content;
    std::string fileName;
    int choose;

    std::cout << "0 for encrypt, 1 for decrypt: ";
    std::cin >> choose;
    std::cin.ignore();

    if (choose == 0) {
        std::cout << "0 for creating a new file 1 for taking a file and encrypting it\n";
        std::cin >> choose;
        std::cin.ignore();
        if (choose == 0) {
            std::cout << "Enter content to encrypt: \n";
            std::getline(std::cin, content);


            std::cout << "Enter filename with extension: \n";
            std::cin >> fileName;
                if (encrypt(fileName, content, 0)) {
                    std::cout << "File encrypted successfully.\n";
                    std::cout << "SAVE THIS key: " << KEY << "\n";
                }

                else {
                    std::cout << "Failed to encrypt the file.\n";
                }
        }
        else if (choose == 1) {
            std::cout << "Enter filename to encrypt with extension: \n";
            std::string fileNameToEncrypt;
            std::cin >> fileNameToEncrypt;

            std::cout << "Enter filename with extension: \n";
            std::cin >> fileName;

            if (encrypt(fileName, fileNameToEncrypt, 1)) {
                std::cout << "File encrypted successfully.\n";
                std::cout << "SAVE THIS key: " << KEY << "\n";
            }

            }
        }
    else if (choose == 1) {
        uint64_t pass;
        std::cout << "Enter filename with extension: \n";
        std::cin >> fileName;
        std::cout << "Enter Key: ";
        std::cin >> pass;
        std::string decryptedContent = decrypt(fileName, pass);
        if (!decryptedContent.empty()) {
            std::cout << "Decrypted content: " << decryptedContent << "\n";
        }
        else {
            std::cout << "Failed to decrypt the file or file is empty.\n";
        }
    }
    std::system("pause");
    return 0;
}
