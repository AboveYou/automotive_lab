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

int fresh_hmac_payload(const CryptoPP::byte *key, size_t key_size,
                       const CryptoPP::byte *payload, size_t payload_size,
                       const CryptoPP::byte *counter, size_t counter_size,
                       CryptoPP::byte *digest) {
    HMAC<SHA256> hmac(key, key_size);

    hmac.Update(payload, payload_size);
    hmac.Update(counter, counter_size);
    hmac.Final(digest);

    // display the HMAC
    cout << "HMAC: ";
    for (size_t i = 0; i < HMAC<SHA256>::DIGESTSIZE; ++i) {
        cout << hex << setw(2) << setfill('0') << static_cast<unsigned int>(digest[i]);
    }
    cout << endl;

    return 0;
}

void verify_frame(const CryptoPP::byte *key,
                  const CryptoPP::byte *counter,
                  const canfd_frame& frame) {
    int message_len = frame.len - 1 - HMAC<SHA256>::DIGESTSIZE;
    CryptoPP::byte message[message_len];
    memcpy(message, frame.data, message_len);

    CryptoPP::byte receivedCounter[counter_size];
    memcpy(&receivedCounter, frame.data + message_len, 1);

    CryptoPP::byte receivedDigest[HMAC<SHA256>::DIGESTSIZE];
    memcpy(receivedDigest, frame.data + message_len + 1, HMAC<SHA256>::DIGESTSIZE);

    // Verify the received message by hashing it and comparing the hash with the received hash
    CryptoPP::byte calculatedDigest[HMAC<SHA256>::DIGESTSIZE];
    fresh_hmac_payload(key, key_size,
                       message, message_len,
                       receivedCounter, counter_size,
                       calculatedDigest);

    // Compare the calculated digest with the received digest
    bool isMessageValid = (memcmp(receivedDigest, calculatedDigest, HMAC<SHA256>::DIGESTSIZE) == 0);
    bool isCounterValid = (memcmp(receivedCounter, counter, 1) == 0);

    if (isMessageValid && isCounterValid) {
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
    CryptoPP::byte counter[] = {
            0x00
    };

    const CryptoPP::byte key[] = {
        0x01, 0x02, 0x03, 0x04,
        0x01, 0x02, 0x03, 0x04,
        0x01, 0x02, 0x03, 0x04,
    };

    if (strcmp(argv[1], "-s") == 0) {
        cout << "acting as sender" << endl;
        CryptoPP::byte message[] = {
            0x64,0x6f,0x20,0x6e,0x6f,0x74,0x20,0x74,
            0x61,0x6c,0x6b,0x20,0x61,0x62,0x6f,0x75,
            0x74,0x20,0x66,0x69,0x67,0x68,0x74,0x20,
            0x63,0x6c,0x75,0x62, 0x21
        };

        CryptoPP::byte digest[HMAC<SHA256>::DIGESTSIZE];

        fresh_hmac_payload(key, sizeof(key),
                           message, sizeof(message),
                           counter, sizeof(counter),
                           digest);

        canfd_frame frame;
        memset(&frame, 0, sizeof(frame));
        frame.can_id = 0x7334;
        frame.len = sizeof(message) + 1 + HMAC<SHA256>::DIGESTSIZE;
        memcpy(frame.data, message, sizeof(message));
        mempcpy(frame.data + sizeof(message), &counter, 1);
        memcpy(frame.data + sizeof(message) + 1, digest, HMAC<SHA256>::DIGESTSIZE);

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
        verify_frame(key, sizeof(key), counter, sizeof(counter), frame);
    }
    else {
        cout << "the parameter is invalid!" << endl;
        print_usage();
        return 1;
    }

    return 0;
}
