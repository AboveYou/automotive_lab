#include "canfd_recv.h"

int main() {
    CANFDReceiver recv("vcan0");
    
    canfd_frame frame;
    
    // prepare the CAN-FD frame
    memset(&frame, 0, sizeof(frame));
    
    if (recv.receiveFrame(frame)) {
        recv.printFrame(frame);
    };

	return 0;
}
