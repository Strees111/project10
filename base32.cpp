#include <iostream>
#include <cstring>
#include <fstream>

char base32Char(unsigned char b) {
    const char base32chars[] = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";
    b &= 0x1F;
    return base32chars[b];
}

unsigned char base32Code(char ch) {
    if (ch >= '0' && ch <= '9') { return ch - '0'; }
    if (ch == '=') { return 32; }
    ch = static_cast<char>(toupper(ch));
    if (ch >= 'A' && ch <= 'H') { return (ch - 'A') + 10; }
    if (ch >= 'J' && ch <= 'K') { return (ch - 'A') + 9; }
    if (ch >= 'M' && ch <= 'N') { return (ch - 'A') + 8; }
    if (ch >= 'P' && ch <= 'T') { return (ch - 'A') + 7; }
    if (ch >= 'V' && ch <= 'Z') { return (ch - 'A') + 6; }
    if (ch == 'I' || ch == 'L') { return 1; }
    return 0;
}

bool isBase32(char ch) {
    if (ch >= '0' && ch <= '9') { return true; }
    if (ch == '=') { return true; }
    ch = static_cast<char>(toupper(ch));
    if (ch >= 'A' && ch <= 'H') { return true; }
    if (ch >= 'J' && ch <= 'K') { return true; }
    if (ch >= 'M' && ch <= 'N') { return true; }
    if (ch >= 'P' && ch <= 'T') { return true; }
    if (ch >= 'V' && ch <= 'Z') { return true; }
    if (ch == 'I' || ch == 'L') { return true; }
    if (ch == 'O') { return true; }
    return false;
}

int base32Decode(const char inStr[], int Len, char outStr[]) {
    int outIndex = 0;
    unsigned char a1;
    unsigned char a2;
    unsigned char a3;
    unsigned char a4;
    unsigned char a5;
    unsigned char a6;
    unsigned char a7;
    unsigned char a8;
    int skip = 0;
    if (Len % 8 != 0) {
        throw std::invalid_argument("Wrong data for decoding");
    }
    for (int inIndex = 0; (inIndex < Len) && (skip == 0); inIndex += 8) {
        a1 = base32Code(inStr[inIndex]);
        a2 = base32Code(inStr[inIndex + 1]);
        a3 = base32Code(inStr[inIndex + 2]);
        a4 = base32Code(inStr[inIndex + 3]);
        a5 = base32Code(inStr[inIndex + 4]);
        a6 = base32Code(inStr[inIndex + 5]);
        a7 = base32Code(inStr[inIndex + 6]);
        a8 = base32Code(inStr[inIndex + 7]);

        if (a8 == 32) {
            a8 = 0;
            skip = 1;
        }
        if (a6 == 32) {
            a6 = a7 = 0;
            skip = 2;
        }
        if (a5 == 32) {
            a5 = 0;
            skip = 3;
        }
        if (a3 == 32) {
            a3 = a4 = 0;
            skip = 4;
        }

        outStr[outIndex++] = static_cast<char>((a1 << 3) | (a2 >> 2));
        outStr[outIndex++] = static_cast<char>(((a2 & 0x3) << 6) | (a3 << 1) | (a4 >> 4));
        outStr[outIndex++] = static_cast<char>(((a4 & 0xF) << 4) | (a5 >> 1));
        outStr[outIndex++] = static_cast<char>(((a5 & 0x1) << 7) | (a6 << 2) | (a7 >> 3));
        outStr[outIndex++] = static_cast<char>(((a7 & 0x7) << 5) | a8);
    }
    return outIndex - skip;
}

int base32Encode(const char inStr[], int Len, char outStr[]) {
    unsigned char a1;
    unsigned char a2;
    unsigned char a3;
    unsigned char a4;
    unsigned char a5;
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
    unsigned char b4;
    unsigned char b5;
    unsigned char b6;
    unsigned char b7;
    unsigned char b8;
    int inIndex = 0;
    int outIndex = 0;
    for (; inIndex < Len; inIndex += 5) {
        a2 = 0;
        a3 = 0;
        a4 = 0;
        a5 = 0;
        a1 = inStr[inIndex];
        if (inIndex + 1 < Len) { a2 = inStr[inIndex + 1]; }
        if (inIndex + 2 < Len) { a3 = inStr[inIndex + 2]; }
        if (inIndex + 3 < Len) { a4 = inStr[inIndex + 3]; }
        if (inIndex + 4 < Len) { a5 = inStr[inIndex + 4]; }

        b1 = a1 >> 3;
        b2 = ((a1 & 0x7) << 2) | (a2 >> 6);
        b3 = ((a2 & 0x3E) >> 1);
        b4 = ((a2 & 0x1) << 4) | ((a3 >> 4));
        b5 = ((a3 & 0xF) << 1) | ((a4 >> 7));
        b6 = ((a4 & 0x7C) >> 2);
        b7 = ((a4 & 0x3) << 3) | ((a5 >> 5));
        b8 = (a5 & 0x1F);

        outStr[outIndex++] = base32Char(b1);
        outStr[outIndex++] = base32Char(b2);
        outStr[outIndex++] = (inIndex + 1 < Len) ? base32Char(b3) : '=';
        outStr[outIndex++] = (inIndex + 1 < Len) ? base32Char(b4) : '=';
        outStr[outIndex++] = (inIndex + 2 < Len) ? base32Char(b5) : '=';
        outStr[outIndex++] = (inIndex + 3 < Len) ? base32Char(b6) : '=';
        outStr[outIndex++] = (inIndex + 3 < Len) ? base32Char(b7) : '=';
        outStr[outIndex++] = (inIndex + 4 < Len) ? base32Char(b8) : '=';
    }
    return outIndex;
}

void code32(std::ofstream &outFile, std::ifstream &inFile) {
    const int block_size = 20;
    char inStr[5 * block_size];
    char outStr[8 * block_size];
    while (inFile.peek() != EOF) {
        inFile.read(inStr, 5 * block_size);
        int s = static_cast<int>(inFile.gcount());
        s = base32Encode(inStr, s, outStr);
        outFile.write(outStr, s);
    }
}

void decode32(std::ofstream &outFile, std::ifstream &inFile) {
    char inStr[8];
    char outStr[5];
    int ch;
    int cnt = 0;

    while ((ch = inFile.get()) != EOF) {
        if (isBase32(static_cast<char>(ch))) {
            inStr[cnt++] = static_cast<char>(ch);
            if (cnt == 8) {
                try {
                    cnt = base32Decode(inStr, 8, outStr);
                    outFile.write(outStr, cnt);
                }
                catch (const char *error) {
                    std::cerr << "Error: " << error << std::endl;
                    return;
                }
                catch (...) {
                    std::cerr << "Unknown error!" << std::endl;
                    return;
                }
                cnt = 0;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Use for encoding:\n";
        std::cout << "  " << argv[0] << " file_source file_destination" << std::endl;
        return 1;
    }
    std::ifstream inFile(argv[2], std::ios_base::binary);
    if (!inFile) {
        std::cout << "Can not open file " << argv[1] << std::endl;
        return 2;
    }
    std::ofstream outFile(argv[3], std::ios_base::binary);
    if (!outFile) {
        std::cout << "Can not open file " << argv[2] << std::endl;
        inFile.close();
        return 3;
    }
    if (strcmp(argv[1], "-e") == 0) {
        code32(outFile, inFile);
    }
    if (strcmp(argv[1], "-d") == 0) {
        decode32(outFile, inFile);
    }
    inFile.close();
    outFile.close();
    return 0;
}