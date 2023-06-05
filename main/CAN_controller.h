#include "NMEA_msg.h"
/**
 * Abstract CAN Controller 
 * Implemented for each specific CAN controller used
*/
class CANController
{
    public:
        virtual ~CANController() {}
        virtual void init() = 0;
        virtual void read(&NMEA_msg msg) = 0;
        virtual void send(int PGN, uint_8[] data ) = 0;
        virtual void send(NMEA_msg msg) = 0;
};
