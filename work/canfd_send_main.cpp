#include "canfd_send.h"

int main() {
    CANFDSender sender("vcan0");

    canfd_frame frame;

    // prepare the CAN-FD frame
    memset(&frame, 0, sizeof(frame));

    frame.can_id = 0x123;
    frame.len = 8;
    memset(frame.data, 0, sizeof(frame.data));

    // Set the payload data
    frame.data[0] = 0x11;
    frame.data[1] = 0x22;
    frame.data[2] = 0x33;
    frame.data[3] = 0x44;
    frame.data[4] = 0x55;
    frame.data[5] = 0x66;
    frame.data[6] = 0x77;
    frame.data[7] = 0x88;

    if (sender.sendFrame(frame)) {
        sender.printFrame(frame);
    };
    
    return 0;
}
