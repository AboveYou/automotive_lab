/**
 * In this file you will get a quaick introduction into the crypto library of C++.
 * We need several aspects of this library throuougt this lab.
 * 
 * - hashing (SHA256)
 * - symmetric keys (AES)
 * - hash messag authentication codes (HMACs)
*/
#include <iostream>
#include <iomanip>
#include <string>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/hmac.h>

using namespace std;
using namespace CryptoPP;

void hashingTest() {
    // the byte array to hash
    CryptoPP::byte input[4] = {0x12, 0x34, 0x56, 0x78};

    // SHA-256 will be used for hashing
    SHA256 hash;
    CryptoPP::byte digest[SHA256::DIGESTSIZE];

    // introduce the array to the class
    hash.Update(input, 4);
    // calculate the hash
    hash.Final(digest);

    // display the hash
    cout << "SHA-256 Hash: ";
    for (size_t i = 0; i < SHA256::DIGESTSIZE; ++i) {
        cout << hex << setw(2) << setfill('0') << static_cast<unsigned int>(digest[i]);
    }
    cout << endl;
}

void hmacTest() {
    CryptoPP::byte key[] = {0x01, 0x02, 0x03, 0x04};
    size_t keySize = sizeof(key);

    CryptoPP::byte data[] = {0x61, 0x62, 0x63, 0x64}; // ASCII values of "abcd"
    size_t dataSize = sizeof(data);

    CryptoPP::byte digest[HMAC<SHA256>::DIGESTSIZE];

    HMAC<SHA256> hmac(key, keySize);
    hmac.Update(data, dataSize);
    hmac.Final(digest);

    string hexString;
    StringSource(digest, sizeof(digest), true,
        new HexEncoder(
            new StringSink(hexString)
        )
    );

    cout << "HMAC: " << hexString << endl;
}

int main () {
    hashingTest();
    hmacTest();
    return 0;
}