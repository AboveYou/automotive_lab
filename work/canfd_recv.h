#ifndef CAN_FD_RECEIVER_H
#define CAN_FD_RECEIVER_H

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

using namespace std;

class CANFDReceiver {
    private:
        int required_mtu;
        int socket_;
        struct sockaddr_can addr_;

    public:
        CANFDReceiver(const char* interface);
        ~CANFDReceiver();

        bool receiveFrame(canfd_frame &frame);
        void printFrame(const canfd_frame& frame);
};

#endif  // CAN_FD_RECEIVER_H
