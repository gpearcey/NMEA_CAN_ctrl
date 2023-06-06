#ifndef MCP2515_CAN_CONTROLLER_H
#define MCP2515_CAN_CONTROLLER_H

#include "CAN_controller.h"
#include "NMEA_msg.h"

class mcp2515CANController : public class CANController
{
    public:
        void init();
        void read(&NMEA_msg msg);
        void send(int PGN, uint_8[] data );
        void send(NMEA_msg msg);
};

#endif //MCP2515_CAN_CONTROLLER