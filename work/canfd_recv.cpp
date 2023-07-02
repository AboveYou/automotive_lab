#include "canfd_recv.h"

CANFDReceiver::CANFDReceiver(const char* interface) {
    // the maximum transfer unit is set
    this->required_mtu = CANFD_MTU;

    // open a SocketCAN socket
    this->socket_ = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (this->socket_ < 0) {
        perror("Error while opening socket");
        return;
    }

    // specify the CAN interface name (e.g., "can0")
    struct ifreq ifr;
    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);
    ioctl(this->socket_, SIOCGIFINDEX, &ifr);

    // bind the socket to the CAN interface
    this->addr_.can_family = AF_CAN;
    this->addr_.can_ifindex = ifr.ifr_ifindex;
    if (bind(this->socket_, reinterpret_cast<struct sockaddr*>(&this->addr_), sizeof(this->addr_)) < 0) {
        perror("Error in socket bind");
        close(this->socket_);
        return;
    }

    // enable CAN FD reception on the socket
    const int enable_canfd = 1;
    if (setsockopt(this->socket_, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable_canfd, sizeof(enable_canfd)) < 0) {
        perror("Error enabling CAN FD reception");
        close(this->socket_);
        return;
    }
}

CANFDReceiver::~CANFDReceiver() {
    // close the socket
    close(this->socket_);
}

bool CANFDReceiver::receiveFrame(canfd_frame &frame) {
    // receive one CAN FD frame
    if (recv(this->socket_, &frame, this->required_mtu, 0) != this->required_mtu) {
        perror("Error while receiving CAN frame");
        close(this->socket_);
        return false;
    }

    return true;
}

void CANFDReceiver::printFrame(const canfd_frame &frame) {
    // print the CAN frame (for debugging)
    cout << "[>] received CAN FD frame" << endl; 
    cout << "\t- ID: 0x" << hex << frame.can_id << endl;
    cout << "\t- Data: ";
    for (int i = 0; i < frame.len; ++i) {
        cout << hex << static_cast<int>(frame.data[i]) << " ";
    }
}
