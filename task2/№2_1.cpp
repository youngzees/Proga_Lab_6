#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <iomanip>
#include <string>

using namespace std;

// S-BOX и INV S-BOX
const unsigned char Sbox[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xff,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

const unsigned char InvSbox[256] = {
    0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
    0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
    0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
    0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
    0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
    0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
    0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
    0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
    0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
    0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
    0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
    0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
    0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
    0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
    0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
    0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};

void printHex(const unsigned char* data, int len, const string& title) {
    cout << title << ": ";
    for (int i = 0; i < len; i++) {
        cout << hex << setw(2) << setfill('0') << (int)data[i];
    }
    cout << dec << endl;
}

void printState(const unsigned char state[4][4], const string& title) {
    cout << title << ":" << endl;
    for (int i = 0; i < 4; i++) {
        cout << "  ";
        for (int j = 0; j < 4; j++) {
            cout << hex << setw(2) << setfill('0') << (int)state[i][j] << " ";
        }
        cout << dec << endl;
    }
}

unsigned char galoisMultiply(unsigned char a, unsigned char b) {
    unsigned char result = 0;
    unsigned char hi_bit;
    for (int i = 0; i < 8; i++) {
        if (b & 1) result ^= a;
        hi_bit = a & 0x80;
        a <<= 1;
        if (hi_bit) a ^= 0x1b;
        b >>= 1;
    }
    return result;
}

void subBytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] = Sbox[state[i][j]];
}

void invSubBytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] = InvSbox[state[i][j]];
}

void shiftRows(unsigned char state[4][4]) {
    unsigned char temp;
    temp = state[1][0]; state[1][0] = state[1][1]; state[1][1] = state[1][2]; state[1][2] = state[1][3]; state[1][3] = temp;
    temp = state[2][0]; state[2][0] = state[2][2]; state[2][2] = temp;
    temp = state[2][1]; state[2][1] = state[2][3]; state[2][3] = temp;
    temp = state[3][3]; state[3][3] = state[3][2]; state[3][2] = state[3][1]; state[3][1] = state[3][0]; state[3][0] = temp;
}

void invShiftRows(unsigned char state[4][4]) {
    unsigned char temp;
    temp = state[1][3]; state[1][3] = state[1][2]; state[1][2] = state[1][1]; state[1][1] = state[1][0]; state[1][0] = temp;
    temp = state[2][0]; state[2][0] = state[2][2]; state[2][2] = temp;
    temp = state[2][1]; state[2][1] = state[2][3]; state[2][3] = temp;
    temp = state[3][0]; state[3][0] = state[3][1]; state[3][1] = state[3][2]; state[3][2] = state[3][3]; state[3][3] = temp;
}

void mixColumns(unsigned char state[4][4]) {
    unsigned char result[4][4];
    for (int c = 0; c < 4; c++) {
        result[0][c] = galoisMultiply(0x02, state[0][c]) ^ galoisMultiply(0x03, state[1][c]) ^ state[2][c] ^ state[3][c];
        result[1][c] = state[0][c] ^ galoisMultiply(0x02, state[1][c]) ^ galoisMultiply(0x03, state[2][c]) ^ state[3][c];
        result[2][c] = state[0][c] ^ state[1][c] ^ galoisMultiply(0x02, state[2][c]) ^ galoisMultiply(0x03, state[3][c]);
        result[3][c] = galoisMultiply(0x03, state[0][c]) ^ state[1][c] ^ state[2][c] ^ galoisMultiply(0x02, state[3][c]);
    }
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] = result[i][j];
}

void invMixColumns(unsigned char state[4][4]) {
    unsigned char result[4][4];
    for (int c = 0; c < 4; c++) {
        result[0][c] = galoisMultiply(0x0e, state[0][c]) ^ galoisMultiply(0x0b, state[1][c]) ^ galoisMultiply(0x0d, state[2][c]) ^ galoisMultiply(0x09, state[3][c]);
        result[1][c] = galoisMultiply(0x09, state[0][c]) ^ galoisMultiply(0x0e, state[1][c]) ^ galoisMultiply(0x0b, state[2][c]) ^ galoisMultiply(0x0d, state[3][c]);
        result[2][c] = galoisMultiply(0x0d, state[0][c]) ^ galoisMultiply(0x09, state[1][c]) ^ galoisMultiply(0x0e, state[2][c]) ^ galoisMultiply(0x0b, state[3][c]);
        result[3][c] = galoisMultiply(0x0b, state[0][c]) ^ galoisMultiply(0x0d, state[1][c]) ^ galoisMultiply(0x09, state[2][c]) ^ galoisMultiply(0x0e, state[3][c]);
    }
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] = result[i][j];
}

void addRoundKey(unsigned char state[4][4], unsigned char roundKey[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] ^= roundKey[i][j];
}

void rotWord(unsigned char word[4]) {
    unsigned char temp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
}

void subWord(unsigned char word[4]) {
    for (int i = 0; i < 4; i++)
        word[i] = Sbox[word[i]];
}

void expandKey(const unsigned char* key, unsigned char roundKeys[11][4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            roundKeys[0][i][j] = key[i * 4 + j];
    
    const unsigned char Rcon[10][4] = {
        {0x01,0x00,0x00,0x00}, {0x02,0x00,0x00,0x00}, {0x04,0x00,0x00,0x00},
        {0x08,0x00,0x00,0x00}, {0x10,0x00,0x00,0x00}, {0x20,0x00,0x00,0x00},
        {0x40,0x00,0x00,0x00}, {0x80,0x00,0x00,0x00}, {0x1b,0x00,0x00,0x00},
        {0x36,0x00,0x00,0x00}
    };
    
    for (int i = 1; i <= 10; i++) {
        unsigned char temp[4];
        for (int j = 0; j < 4; j++) temp[j] = roundKeys[i-1][j][3];
        
        rotWord(temp);
        subWord(temp);
        
        for (int j = 0; j < 4; j++) temp[j] ^= Rcon[i-1][j];
        
        for (int j = 0; j < 4; j++) {
            roundKeys[i][j][0] = roundKeys[i-1][j][0] ^ temp[j];
            roundKeys[i][j][1] = roundKeys[i-1][j][1] ^ roundKeys[i][j][0];
            roundKeys[i][j][2] = roundKeys[i-1][j][2] ^ roundKeys[i][j][1];
            roundKeys[i][j][3] = roundKeys[i-1][j][3] ^ roundKeys[i][j][2];
        }
    }
    
    cout << "\n ВСЕ РАУНДОВЫЕ КЛЮЧИ " << endl;
    for (int r = 0; r <= 10; r++) {
        unsigned char flat[16];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                flat[i*4 + j] = roundKeys[r][i][j];
        printHex(flat, 16, "RoundKey " + to_string(r));
    }
}

void encryptBlock(unsigned char state[4][4], unsigned char roundKeys[11][4][4]) {
    cout << "\n  Начальный State:" << endl;
    printState(state, "    ");
    
    addRoundKey(state, roundKeys[0]);
    printState(state, "  После AddRoundKey[0]");
    
    for (int round = 1; round <= 9; round++) {
        cout << "\n Раунд " << round << endl;
        
        subBytes(state);
        printState(state, "    SubBytes");
        
        shiftRows(state);
        printState(state, "    ShiftRows");
        
        mixColumns(state);
        printState(state, "    MixColumns");
        
        addRoundKey(state, roundKeys[round]);
        printState(state, "    AddRoundKey");
    }
    
    cout << "\n  Последний раунд (10)" << endl;
    subBytes(state);
    printState(state, "    SubBytes");
    
    shiftRows(state);
    printState(state, "    ShiftRows");
    
    addRoundKey(state, roundKeys[10]);
    printState(state, "    AddRoundKey");
}

void decryptBlock(unsigned char state[4][4], unsigned char roundKeys[11][4][4]) {
    addRoundKey(state, roundKeys[10]);
    
    for (int round = 9; round >= 1; round--) {
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state, roundKeys[round]);
        invMixColumns(state);
    }
    
    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state, roundKeys[0]);
}

vector<unsigned char> pkcs7Pad(const vector<unsigned char>& data, int blockSize) {
    int padding = blockSize - (data.size() % blockSize);
    vector<unsigned char> padded = data;
    for (int i = 0; i < padding; i++) {
        padded.push_back((unsigned char)padding);
    }
    return padded;
}

vector<unsigned char> pkcs7Unpad(const vector<unsigned char>& data) {
    if (data.empty()) return data;
    int padding = data.back();
    if (padding > 16 || padding < 1 || padding > (int)data.size()) return data;
    for (int i = data.size() - padding; i < (int)data.size(); i++) {
        if (data[i] != padding) return data;
    }
    return vector<unsigned char>(data.begin(), data.end() - padding);
}

vector<unsigned char> ofbEncrypt(const vector<unsigned char>& plaintext, 
                                   unsigned char roundKeys[11][4][4],
                                   unsigned char iv[16]) {
    vector<unsigned char> ciphertext;
    unsigned char feedback[4][4];
    unsigned char keystream[4][4];
    unsigned char block[4][4];
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            feedback[i][j] = iv[i*4 + j];
    
    int numBlocks = plaintext.size() / 16;
    for (int blockNum = 0; blockNum < numBlocks; blockNum++) {
        cout << "\n БЛОК " << blockNum << " (шифрование) " << endl;
        
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                keystream[i][j] = feedback[i][j];
        
        encryptBlock(keystream, roundKeys);
        
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                feedback[i][j] = keystream[i][j];
        
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                block[i][j] = plaintext[blockNum*16 + i*4 + j];
        
        cout << "\n  Блок открытого текста:" << endl;
        printState(block, "    ");
        
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                ciphertext.push_back(block[i][j] ^ keystream[i][j]);
        
        cout << "\n  Результат XOR (шифротекст блока):" << endl;
        unsigned char temp[4][4];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp[i][j] = block[i][j] ^ keystream[i][j];
        printState(temp, "    ");
    }
    
    return ciphertext;
}

vector<unsigned char> ofbDecrypt(const vector<unsigned char>& ciphertext,
                                   unsigned char roundKeys[11][4][4],
                                   unsigned char iv[16]) {
    return ofbEncrypt(ciphertext, roundKeys, iv);
}

void generateRandomKey(unsigned char* key, int size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    for (int i = 0; i < size; i++)
        key[i] = dis(gen);
}

void saveKeyToFile(const unsigned char* key, const string& filename) {
    ofstream file(filename, ios::binary);
    file.write(reinterpret_cast<const char*>(key), 16);
    file.close();
    cout << "Ключ сохранён в файл: " << filename << endl;
}

int main() {
    cout << "AES-128 OFB шифрование/расшифрование" << endl;
    
    unsigned char key[16];
    generateRandomKey(key, 16);
    printHex(key, 16, "\nСгенерированный ключ");
    saveKeyToFile(key, "aes_key.bin");
    
    unsigned char iv[16];
    generateRandomKey(iv, 16);
    printHex(iv, 16, "\nВектор инициализации (IV)");
    
    unsigned char roundKeys[11][4][4];
    expandKey(key, roundKeys);
    
    cout << "\nВведите текст для шифрования: ";
    string plaintextStr;
    getline(cin, plaintextStr);
    
    vector<unsigned char> plaintext;
    for (char c : plaintextStr) {
        plaintext.push_back((unsigned char)c);
    }
    
    vector<unsigned char> paddedPlaintext = pkcs7Pad(plaintext, 16);
    
    cout << "\nИсходный текст: " << plaintextStr << endl;
    cout << "Размер исходного текста: " << plaintext.size() << " байт" << endl;
    cout << "После PKCS#7 паддинга: " << paddedPlaintext.size() << " байт" << endl;
    
    // Шифрование (пункт а)
    vector<unsigned char> ciphertext = ofbEncrypt(paddedPlaintext, roundKeys, iv);
    cout << "\n ЗАШИФРОВАННЫЕ ДАННЫЕ (hex) " << endl;
    cout << "Полный шифротекст: ";
    for (unsigned char c : ciphertext) {
        cout << hex << setw(2) << setfill('0') << (int)c;
    }
    cout << dec << endl;
    
    // Расшифрование (пункт б)
    cout << "\n\n РАСШИФРОВАНИЕ " << endl;
    vector<unsigned char> decryptedPadded = ofbDecrypt(ciphertext, roundKeys, iv);
    vector<unsigned char> decrypted = pkcs7Unpad(decryptedPadded);
    
    string decryptedStr;
    for (unsigned char c : decrypted) {
        decryptedStr += (char)c;
    }
    
    cout << "\nРасшифрованный текст: " << decryptedStr << endl;
    
    if (plaintextStr == decryptedStr) {
        cout << "\nШифрование и расшифрование выполнены успешно!" << endl;
    } else {
        cout << "\nОшибка: текст не совпадает!" << endl;
    }
    
    return 0;
}