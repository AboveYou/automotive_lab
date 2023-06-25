#include <iostream>
#include <cryptopp/sha.h>
#include <iomanip>
#include <string>
#include "canfd_send.h"
#include "canfd_recv.h"

using namespace std;
using namespace CryptoPP;

int hash_payload(const CryptoPP::byte *payload, size_t payload_size, CryptoPP::byte *digest) {
    // hashing using SHA-256
    SHA256 hash;

    hash.Update(payload, payload_size);
    hash.Final(digest);

     // display the hash result
     cout << "Hash: ";
     for (size_t i = 0; i < SHA256::DIGESTSIZE; ++i) {
         cout << hex << setw(2) << setfill('0') << static_cast<unsigned int>(digest[i]);
     }
    cout << endl << endl;

    return 0;

}

bool verify_frame(const canfd_frame& frame) {
    int message_len = frame.len - SHA256::DIGESTSIZE;
    CryptoPP::byte message[message_len];
    memcpy(message, &frame.data, message_len);

    CryptoPP::byte receivedDigest[SHA256::DIGESTSIZE];
    memcpy(receivedDigest, frame.data + message_len, SHA256::DIGESTSIZE);

    CryptoPP::byte calculatedDigest[SHA256::DIGESTSIZE];
    hash_payload(message, message_len, calculatedDigest);

    bool isMessageValid = (memcmp(receivedDigest, calculatedDigest, SHA256::DIGESTSIZE) == 0);

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

    if (strcmp(argv[1], "-s") == 0) {
        cout << "[>] acting as sender" << endl << endl;
        CryptoPP::byte message[] = {
                0x66,0x6f,0x6c,0x6c,0x6f,0x77,0x20,0x74,
                0x68,0x65,0x20,0x77,0x68,0x69,0x74,0x65,
                0x20,0x72,0x61,0x62,0x62,0x69,0x74
        };

        CryptoPP::byte digest[SHA256::DIGESTSIZE];

        hash_payload(message, sizeof(message), digest);

        canfd_frame frame;
        memset(&frame, 0, sizeof(frame));
        frame.can_id = 0x7334;
        frame.len = sizeof(message) + SHA256::DIGESTSIZE;
        memcpy(frame.data, message, sizeof(message));
        memcpy(frame.data + sizeof(message), digest, SHA256::DIGESTSIZE);

        CANFDSender sender("vcan0");

        sender.sendFrame(frame);
        sender.printFrame(frame);
    }
    else if (strcmp(argv[1], "-r") == 0) {
        cout << "[>] acting as receiver" << endl << endl;
        canfd_frame frame;
        memset(&frame, 0, sizeof(frame));

        CANFDReceiver receiver("vcan0");

        receiver.receiveFrame(frame);
        verify_frame(frame);
    }
    else {
        cout << "[!] the parameter is invalid!" << endl;
        print_usage();
        return 1;
    }

    return 0;
}
