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

int fresh_hmac_payload(const CryptoPP::byte *key, const size_t key_size,
                       const CryptoPP::byte *payload, const size_t payload_size,
                       const CryptoPP::byte *counter, const size_t counter_size,
                       CryptoPP::byte *digest) {

    // TODO insert code here

    // display the HMAC
    cout << "HMAC (with freshness): ";
    for (size_t i = 0; i < HMAC<SHA256>::DIGESTSIZE; ++i) {
        cout << hex << setw(2) << setfill('0') << static_cast<unsigned int>(digest[i]);
    }
    cout << endl << endl;

    return 0;
}

bool verify_frame(const CryptoPP::byte *key, const size_t key_size,
                  const CryptoPP::byte *counter, const size_t counter_size,
                  const canfd_frame& frame) {

    bool isMessageValid;
    bool isCounterValid;

    // TODO inset code here

    if (isMessageValid) {
        cout << "[+] counter valid" << endl;
    }

    if (isCounterValid) {
        cout << "[+] counter valid" << endl;
    }

    if (!isMessageValid || !isCounterValid) {
        cout << "[-] received message invalid" << endl;
        return false;
    }

    return true;
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

    canfd_frame frame;
    memset(&frame, 0, sizeof(frame));

    if (strcmp(argv[1], "-s") == 0) {
        cout << "[>] acting as sender" << endl << endl;

        // TODO insert code here

    }
    else if (strcmp(argv[1], "-r") == 0) {
        cout << "[>] acting as receiver" << endl << endl;

        // TODO insert code here

    }
    else {
        cout << "[!] the parameter is invalid!" << endl;
        print_usage();
        return 1;
    }

    return 0;
}
