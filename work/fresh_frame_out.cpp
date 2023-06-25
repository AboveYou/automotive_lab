#include <iostream>
#include <iomanip>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

#include "canfd_send.h"

using namespace CryptoPP;
using namespace std;

int fresh_hmac_payload(CryptoPP::byte *key, CryptoPP::byte *payload, CryptoPP::byte *counter, CryptoPP::byte *digest) {
    HMAC<SHA256> hmac(key, sizeof(key));
    
    hmac.Update(payload, sizeof(payload));
    hmac.Update(counter, sizeof(counter));
    hmac.Final(digest);

    // display the HMAC
    cout << "HMAC: ";
    for (size_t i = 0; i < SHA256::DIGESTSIZE; ++i) {
        cout << hex << setw(2) << setfill('0') << static_cast<unsigned int>(digest[i]);
    }
    cout << endl;

    return 0;
}

int main() {
    CryptoPP::byte counter = 0x00;

    CryptoPP::byte key[] = {
        0x01, 0x02, 0x03, 0x04,
        0x01, 0x02, 0x03, 0x04,
        0x01, 0x02, 0x03, 0x04,
    };

    CryptoPP::byte message[] = {
        0x64,0x6f,0x20,0x6e,0x6f,0x74,0x20,0x74,
        0x61,0x6c,0x6b,0x20,0x61,0x62,0x6f,0x75,
        0x74,0x20,0x66,0x69,0x67,0x68,0x74,0x20,
        0x63,0x6c,0x75,0x62
    };

    CryptoPP::byte digest[HMAC<SHA256>::DIGESTSIZE];

    fresh_hmac_payload(key, message, &counter, digest);

    canfd_frame frame;
    memset(&frame, 0, sizeof(frame));
    frame.can_id = 0x7334;
    frame.len = sizeof(message) + SHA256::DIGESTSIZE;
    memcpy(frame.data, message, sizeof(message));
    mempcpy(frame.data + sizeof(message), &counter, 1);
    memcpy(frame.data + sizeof(message) + 1, digest, HMAC<SHA256>::DIGESTSIZE);

    CANFDSender sender("vcan0");
    
    sender.sendFrame(frame);
    sender.printFrame(frame);

    return 0;
}
