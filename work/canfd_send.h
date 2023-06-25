#ifndef CAN_FD_SENDER_H
#define CAN_FD_SENDER_H

#include <iostream>
#include <cstdint>
#include <cstring>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>

using namespace std;

class CANFDSender {
    private:
        int required_mtu;
        int socket_;
        struct sockaddr_can addr_;

    public:
        CANFDSender(const char* interface);
        ~CANFDSender();

        bool sendFrame(const canfd_frame& frame);
        void printFrame(const canfd_frame& frame);
};

#endif  // CAN_FD_SENDER_H
