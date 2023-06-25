#include "canfd_send.h"

int main() {
    CANFDSender sender("vcan0");

    canfd_frame frame;

    // prepare the CAN-FD frame
    memset(&frame, 0, sizeof(frame));

    frame.can_id = 0x123;
    frame.len = 8;
    memset(frame.data, 0, sizeof(frame.data));

    // set the payload data
    frame.data[0] = 0x12;
    frame.data[1] = 0x34;
    frame.data[2] = 0x56;
    frame.data[3] = 0x78;
    frame.data[4] = 0x90;
    frame.data[5] = 0x98;
    frame.data[6] = 0x76;
    frame.data[7] = 0x54;

    if (sender.sendFrame(frame)) {
        sender.printFrame(frame);
    };
    
    return 0;
}
