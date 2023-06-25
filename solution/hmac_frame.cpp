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

int hmac_payload(const CryptoPP::byte *key, const size_t key_size,
                 CryptoPP::byte *payload, const size_t payload_size,
                 CryptoPP::byte *digest) {
    HMAC<SHA256> hmac(key, key_size);
    
    hmac.Update(payload, payload_size);
    hmac.Final(digest);

    // display the HMAC
    cout << "HMAC: ";
    for (size_t i = 0; i < HMAC<SHA256>::DIGESTSIZE; ++i) {
        cout << hex << setw(2) << setfill('0') << static_cast<unsigned int>(digest[i]);
    }
    cout << endl << endl;

    return 0;
}

bool verify_frame(const CryptoPP::byte *key, const size_t key_size, const canfd_frame& frame) {
    int message_len = frame.len - HMAC<SHA256>::DIGESTSIZE;
    CryptoPP::byte message[message_len];
    memcpy(message, &frame.data, message_len);

    CryptoPP::byte receivedDigest[HMAC<SHA256>::DIGESTSIZE];
    memcpy(receivedDigest, frame.data + message_len, HMAC<SHA256>::DIGESTSIZE);

    CryptoPP::byte calculatedDigest[HMAC<SHA256>::DIGESTSIZE];
    hmac_payload(key, key_size, message, message_len, calculatedDigest);

    bool isMessageValid = (memcmp(receivedDigest, calculatedDigest, HMAC<SHA256>::DIGESTSIZE) == 0);

    if (isMessageValid) {
        cout << "[+] received message valid" << endl;
        return true;
    } else {
        cout << "[-] received message invalid" << endl;
        return false;
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

    canfd_frame frame;
    memset(&frame, 0, sizeof(frame));

    if (strcmp(argv[1], "-s") == 0) {
        cout << "[>] acting as sender" << endl << endl;
        CryptoPP::byte message[] = {
                0x68, 0x61, 0x63, 0x6b, 0x20,
                0x74, 0x68, 0x65, 0x20, 0x70,
                0x6c, 0x61, 0x6e, 0x65, 0x74
        };

        CryptoPP::byte digest[HMAC<SHA256>::DIGESTSIZE];

        hmac_payload(key, sizeof(key), message, sizeof(message), digest);

        frame.can_id = 0x7334;
        frame.len = sizeof(message) + HMAC<SHA256>::DIGESTSIZE;
        memcpy(frame.data, message, sizeof(message));
        memcpy(frame.data + sizeof(message), digest, HMAC<SHA256>::DIGESTSIZE);

        CANFDSender sender("vcan0");

        sender.sendFrame(frame);
        sender.printFrame(frame);
    }
    else if (strcmp(argv[1], "-r") == 0) {
        cout << "[>] acting as receiver" << endl << endl;

        CANFDReceiver receiver("vcan0");

        receiver.receiveFrame(frame);
        verify_frame(key, sizeof(key), frame);
    }
    else {
        cout << "[!] the parameter is invalid!" << endl;
        print_usage();
        return 1;
    }

    return 0;
}
