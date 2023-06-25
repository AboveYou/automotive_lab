#include <iostream>
#include <iomanip>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

#include "canfd_recv.h"

using namespace CryptoPP;
using namespace std;

int fresh_hmac_payload(CryptoPP::byte *key, CryptoPP::byte *payload, CryptoPP::byte *counter, CryptoPP::byte *digest) {
    HMAC<SHA256> hmac(key, sizeof(key));
    
    hmac.Update(payload, sizeof(payload));
    hmac.Update(counter, sizeof(counter));
    hmac.Final(digest);

    // display the HMAC
    cout << "HMAC: ";
    for (size_t i = 0; i < HMAC<SHA256>::DIGESTSIZE; ++i) {
        cout << hex << setw(2) << setfill('0') << static_cast<unsigned int>(digest[i]);
    }
    cout << endl;

    return 0;
}

void verify_frame(CryptoPP::byte *key, CryptoPP::byte counter, const canfd_frame& frame) {
    int message_len = frame.len - HMAC<SHA256>::DIGESTSIZE;
    CryptoPP::byte message[message_len];
    memcpy(message, &frame.data, message_len);

    CryptoPP::byte receivedCounter;
    memcpy(&receivedCounter, frame.data + message_len, 1);

    CryptoPP::byte receivedDigest[HMAC<SHA256>::DIGESTSIZE];
    memcpy(receivedDigest, frame.data + message_len + 1, HMAC<SHA256>::DIGESTSIZE);

    // Verify the received message by hashing it and comparing the hash with the received hash
    CryptoPP::byte calculatedDigest[HMAC<SHA256>::DIGESTSIZE];
    fresh_hmac_payload(key, message, &receivedCounter, calculatedDigest);

    // Compare the calculated digest with the received digest
    bool isMessageValid = (memcmp(receivedDigest, calculatedDigest, HMAC<SHA256>::DIGESTSIZE) == 0);
    bool isCounterValid = (memcmp(&receivedCounter, &counter, 1) == 0);

    if (isMessageValid && isCounterValid) {
        cout << "Received message is valid." << endl;
        // Additional processing or actions can be performed here
    } else {
        cout << "Received message is invalid." << endl;
        // Additional handling for invalid messages can be added here
    }
}

int main() {
    CryptoPP::byte counter = 0x00;

    CryptoPP::byte key[] = {
        0x01, 0x02, 0x03, 0x04,
        0x01, 0x02, 0x03, 0x04,
        0x01, 0x02, 0x03, 0x04,
    };

    canfd_frame frame;
    memset(&frame, 0, sizeof(frame));

    CANFDReceiver receiver("vcan0");
    
    receiver.receiveFrame(frame);
    verify_frame(key, counter, frame);

    return 0;
}
