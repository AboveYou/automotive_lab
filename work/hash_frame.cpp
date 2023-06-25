#include <iostream>
#include <cryptopp/sha.h>
#include <iomanip>
#include <string>
#include "canfd_send.h"
#include "canfd_recv.h"

using namespace std;
using namespace CryptoPP;

int hash_payload(const CryptoPP::byte *payload, const size_t payload_size, CryptoPP::byte *digest) {

    // TODO insert code here

     // display the hash result
     cout << "Hash: ";
     for (size_t i = 0; i < SHA256::DIGESTSIZE; ++i) {
         cout << hex << setw(2) << setfill('0') << static_cast<unsigned int>(digest[i]);
     }
    cout << endl << endl;

    return 0;

}

bool verify_frame(const canfd_frame& frame) {

    bool isMessageValid;

    // TODO insert code here

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
