#include <iostream>
#include <iomanip>
#include <string>
#include <cryptopp/sha.h>

#include "canfd_recv.h"

using namespace std;
using namespace CryptoPP;

int hash_payload(const CryptoPP::byte *payload, CryptoPP::byte *digest) {
    // hashing using SHA-256
    SHA256 hash;

    hash.Update(payload, sizeof(payload));
    hash.Final(digest);

    return 0;
}

void verify_frame(const canfd_frame& frame) {
    int message_len = frame.len - SHA256::DIGESTSIZE;
    CryptoPP::byte message[message_len];
    memcpy(message, &frame.data, message_len);

    CryptoPP::byte receivedDigest[SHA256::DIGESTSIZE];
    memcpy(receivedDigest, frame.data + message_len, SHA256::DIGESTSIZE);

    // Verify the received message by hashing it and comparing the hash with the received hash
    CryptoPP::byte calculatedDigest[SHA256::DIGESTSIZE];
    hash_payload(message, calculatedDigest);

    // Compare the calculated digest with the received digest
    bool isMessageValid = (memcmp(receivedDigest, calculatedDigest, SHA256::DIGESTSIZE) == 0);

    if (isMessageValid) {
        cout << "Received message is valid." << endl;
        // Additional processing or actions can be performed here
    } else {
        cout << "Received message is invalid." << endl;
        // Additional handling for invalid messages can be added here
    }
}

int main() {
    canfd_frame frame;
    memset(&frame, 0, sizeof(frame));
    
    CANFDReceiver receiver("vcan0");

    // Simulating the receiving side
    // receiver.receiveFrame(frame);
    receiver.receiveFrame(frame);
    verify_frame(frame);

    return 0;
}
