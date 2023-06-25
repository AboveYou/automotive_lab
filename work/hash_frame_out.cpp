#include <iostream>
#include <iomanip>
#include <string>
#include <cryptopp/sha.h>

#include "canfd_send.h"

using namespace std;
using namespace CryptoPP;

int hash_payload(const CryptoPP::byte *payload, CryptoPP::byte *digest) {
    // hashing using SHA-256
    SHA256 hash;

    hash.Update(payload, sizeof(payload));
    hash.Final(digest);

    // // Display the hash result
    // cout << "Hash: ";
    // for (size_t i = 0; i < SHA256::DIGESTSIZE; ++i) {
    //     cout << hex << setw(2) << setfill('0') << static_cast<unsigned int>(digest[i]);
    // }
    // cout << endl;

    return 0;

}

int main() {
    CryptoPP::byte message[] = {
        0x66,0x6f,0x6c,0x6c,0x6f,0x77,0x20,0x74,
        0x68,0x65,0x20,0x77,0x68,0x69,0x74,0x65,
        0x20,0x72,0x61,0x62,0x62,0x69,0x74
    };
    
    CryptoPP::byte digest[SHA256::DIGESTSIZE];

    hash_payload(message, digest);

    canfd_frame frame;
    memset(&frame, 0, sizeof(frame));
    frame.can_id = 0x7334;
    frame.len = sizeof(message) + SHA256::DIGESTSIZE;
    memcpy(frame.data, message, sizeof(message));
    memcpy(frame.data + sizeof(message), digest, SHA256::DIGESTSIZE);

    CANFDSender sender("vcan0");

    sender.sendFrame(frame);
    sender.printFrame(frame);

    return 0;
}
