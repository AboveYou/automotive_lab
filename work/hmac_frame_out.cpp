#include <iostream>
#include <iomanip>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

#include "canfd_send.h"

using namespace CryptoPP;
using namespace std;

int hmac_payload(CryptoPP::byte *key, CryptoPP::byte *payload, CryptoPP::byte *digest) {
    HMAC<SHA256> hmac(key, sizeof(key));
    
    hmac.Update(payload, sizeof(payload));
    hmac.Final(digest);

    // display the HMAC
    cout << "HMAC: ";
    for (size_t i = 0; i < HMAC<SHA256>::DIGESTSIZE; ++i) {
        cout << hex << setw(2) << setfill('0') << static_cast<unsigned int>(digest[i]);
    }
    cout << endl;

    return 0;
}

int main() {
    CryptoPP::byte key[] = {
        0x01, 0x02, 0x03, 0x04,
        0x01, 0x02, 0x03, 0x04,
        0x01, 0x02, 0x03, 0x04,
    };

    CryptoPP::byte message[] = {
        0x68,0x61,0x63,0x6b,0x20,
        0x74,0x68,0x65,0x20,0x70,
        0x6c,0x61,0x6e,0x65,0x74
    };

    CryptoPP::byte digest[HMAC<SHA256>::DIGESTSIZE];

    hmac_payload(key, message, digest);

    canfd_frame frame;
    memset(&frame, 0, sizeof(frame));
    frame.can_id = 0x7334;
    frame.len = sizeof(message) + HMAC<SHA256>::DIGESTSIZE;
    memcpy(frame.data, message, sizeof(message));
    memcpy(frame.data + sizeof(message), digest, HMAC<SHA256>::DIGESTSIZE);

    CANFDSender sender("vcan0");
    
    sender.sendFrame(frame);
    sender.printFrame(frame);

    return 0;
}
