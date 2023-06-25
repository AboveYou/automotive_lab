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
    string hexString;
    StringSource(digest, sizeof(digest), true,
                 new HexEncoder(
                         new StringSink(hexString)
                 )
    );
    cout << "Hash: " << hexString << endl;
}

void hmacTest() {
    // the key which will be used
    CryptoPP::byte key[] = {0x01, 0x02, 0x03, 0x04};
    size_t keySize = sizeof(key);

    // the data to create a hmac
    CryptoPP::byte data[] = {0x61, 0x62, 0x63, 0x64};
    size_t dataSize = sizeof(data);

    CryptoPP::byte digest[HMAC<SHA256>::DIGESTSIZE];

    // create an object & introduce the key
    HMAC<SHA256> hmac(key, keySize);
    // introduce the array to the class

    hmac.Update(data, dataSize);
    // calculate the hmac
    hmac.Final(digest);

    // display the hmac
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