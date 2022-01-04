#include <iostream>
#include <string>
#include <fstream>
#include <vector>

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        std::cerr << "Error: Use three parameters\n";
        return 5;
    }
    const std::string mode(argv[1]); // Режим работы
    const std::string file_name(argv[2]); // Имя файла


    if (mode == "encryption") {
        // Режим шифрование
        std::cout << "Input message:";
        std::string str;
        getline(std::cin, str);
        std::cout << " " << std::endl;
        std::cout << "Input key:";
        int key;
        std::cin >> key;
        const char *data = str.data();
        size_t size_of_str = str.size();
        std::vector<char> bytes(size_of_str);
        for (int i = 0; i < size_of_str; i++) {
            bytes[i] = data[i];
        }
        srand(key);
        unsigned int rightshift = 1;

        std::ofstream outputFile;
        outputFile.open(file_name, std::ios::binary);
        outputFile.clear();
        for (int i = 0; i < bytes.size(); i += 4) {
            unsigned int gamma = rand();
            unsigned char b0 = bytes[i];
            unsigned char b1 = i + 1 < bytes.size() ? bytes[i + 1] : 0u;
            unsigned char b2 = i + 2 < bytes.size() ? bytes[i + 2] : 0u;
            unsigned char b3 = i + 3 < bytes.size() ? bytes[i + 3] : 0u;
            unsigned int message_to_int =
                    ((static_cast<unsigned int>(b0) << 24u) | (static_cast<unsigned int>(b1) << 16u) |
                     (static_cast<unsigned int>(b2) << 8u) | (static_cast<unsigned int>(b3)));
            unsigned int xored_message = message_to_int ^ gamma;
            unsigned int tmp = (xored_message & 0xFFFFFFFF) << (32 - rightshift);
            unsigned int encrypted_block =
                    (xored_message << rightshift) | tmp;
            char r1 = encrypted_block << 24;
            char r2 = encrypted_block << 16;
            char r3 = encrypted_block << 8;
            char r4 = encrypted_block;
            outputFile.write(&r1, sizeof(r1));
            outputFile.write(&r2, sizeof(r2));
            outputFile.write(&r3, sizeof(r3));
            outputFile.write(&r4, sizeof(r4));
        }
        outputFile.close();


    } else if (mode == "decryption") {

        // Режим расшифрования
        std::cout << "Input key:";
        int key;
        unsigned int rightshift = 1;
        std::cin >> key;
        srand(key);
        std::ifstream readFile;
        readFile.open(file_name, std::ios::binary);

        std::vector<char> encryptedData((std::istreambuf_iterator<char>(readFile)),
                                        std::istreambuf_iterator<char>());
        readFile.close();

        std::vector<char> decryptedData(encryptedData.size());

        for (int i = 0; i < encryptedData.size(); i += 4) {
            unsigned int gamma = rand();
            unsigned char b0 = encryptedData[i];
            unsigned char b1 = i + 1 < encryptedData.size() ? encryptedData[i + 1] : 0u;
            unsigned char b2 = i + 2 < encryptedData.size() ? encryptedData[i + 2] : 0u;
            unsigned char b3 = i + 3 < encryptedData.size() ? encryptedData[i + 3] : 0u;
            unsigned int encrypted_block = ((static_cast<unsigned int>(b0) << 24u) |
                                            (static_cast<unsigned int>(b1) << 16u) |
                                            (static_cast<unsigned int>(b2) << 8u) | (static_cast<unsigned int>(b3)));
            unsigned int shifted_encrypted_block =
                    ((encrypted_block & 0xFFFFFFFF) << rightshift) | encrypted_block >> (32 - rightshift);
            unsigned int result =
                    shifted_encrypted_block ^ gamma;
            unsigned char r0 = result >> 24;
            unsigned char r1 = result >> 16;
            unsigned char r2 = result >> 8;
            unsigned char r3 = result;
            decryptedData[i] = r0;
            decryptedData[i + 1] = r1;
            decryptedData[i + 2] = r2;
            decryptedData[i + 3] = r3;
            for (unsigned int i = 0; i < decryptedData.size(); i++) {
                if (decryptedData[i] != 0) {
                    std::cout << decryptedData[i];
                }
            }
        }
    }
    return 0;
}