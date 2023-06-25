#include "canfd_send.h"

CANFDSender::CANFDSender(const char* interface) {
    // the maximum transfer unit is set
    this->required_mtu = CANFD_MTU;
    
    // open a SocketCAN socket
    this->socket_ = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (this->socket_ < 0) {
        perror("Error while opening socket");
        return;
    }

    // specify the CAN interface name (e.g., "can0")
    struct ifreq ifr{};
    strcpy(ifr.ifr_name, interface);
    ioctl(this->socket_, SIOCGIFINDEX, &ifr);

    // set the CAN interface index
    this->addr_.can_family = AF_CAN;
    this->addr_.can_ifindex = ifr.ifr_ifindex;

    // bind the socket to the CAN interface
    if (bind(this->socket_, reinterpret_cast<struct sockaddr*>(&this->addr_), sizeof(this->addr_)) < 0) {
        perror("Error in socket bind");
        close(this->socket_);
        return;
    }

    // manipulate the socket to enable CAN-FD
    const int enable_canfd = 1;
    setsockopt(this->socket_, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable_canfd, sizeof(enable_canfd));
}

CANFDSender::~CANFDSender() {
    // close the socket
    close(this->socket_);
}

bool CANFDSender::sendFrame(const canfd_frame &frame) {
    // send the CAN-FD frame
    if (write(this->socket_, &frame, this->required_mtu) != this->required_mtu) {
        perror("Error while sending CAN-FD frame");
        close(this->socket_);
        return false;
    }

    return true;
}

void CANFDSender::printFrame(const canfd_frame &frame) {
    cout << "[>] sending CAN FD frame" << endl; 
    cout << "\t- ID: 0x" << hex << frame.can_id << endl;
    cout << "\t- Data: ";
    for (int i = 0; i < frame.len; ++i) {
        cout << hex << static_cast<int>(frame.data[i]) << " ";
    }
}
