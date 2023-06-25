#include <iostream>
#include <iomanip>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

#include "canfd_send.h"
#include "canfd_recv.h"

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

void verify_frame(CryptoPP::byte *key, const canfd_frame& frame) {
    int message_len = frame.len - HMAC<SHA256>::DIGESTSIZE;
    CryptoPP::byte message[message_len];
    memcpy(message, &frame.data, message_len);

    CryptoPP::byte receivedDigest[HMAC<SHA256>::DIGESTSIZE];
    memcpy(receivedDigest, frame.data + message_len, HMAC<SHA256>::DIGESTSIZE);

    // Verify the received message by hashing it and comparing the hash with the received hash
    CryptoPP::byte calculatedDigest[HMAC<SHA256>::DIGESTSIZE];
    hmac_payload(key, message, calculatedDigest);

    // Compare the calculated digest with the received digest
    bool isMessageValid = (memcmp(receivedDigest, calculatedDigest, HMAC<SHA256>::DIGESTSIZE) == 0);

    if (isMessageValid) {
        cout << "Received message is valid." << endl;
        // Additional processing or actions can be performed here
    } else {
        cout << "Received message is invalid." << endl;
        // Additional handling for invalid messages can be added here
    }
}

void print_usage() {
    cout << "usage:" << endl << "\t-s = send" << endl << "\t-r = recv" << endl;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        print_usage();
        return 1;
    }

    CryptoPP::byte key[] = {
            0x01, 0x02, 0x03, 0x04,
            0x01, 0x02, 0x03, 0x04,
            0x01, 0x02, 0x03, 0x04,
    };

    if (strcmp(argv[1], "-s") == 0) {
        cout << "acting as sender" << endl;
        CryptoPP::byte message[] = {
                0x68, 0x61, 0x63, 0x6b, 0x20,
                0x74, 0x68, 0x65, 0x20, 0x70,
                0x6c, 0x61, 0x6e, 0x65, 0x74
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
    }
    else if (strcmp(argv[1], "-r") == 0) {
        cout << "acting as receiver" << endl;
        canfd_frame frame;
        memset(&frame, 0, sizeof(frame));

        CANFDReceiver receiver("vcan0");

        receiver.receiveFrame(frame);
        verify_frame(key, frame);
    }
    else {
        cout << "the parameter is invalid!" << endl;
        print_usage();
        return 1;
    }

    return 0;
}
