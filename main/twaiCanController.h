#include "CAN_controller.h"
#include "NMEA_msg.h"

class twaiCANController : public class CANController
{
    public:
        void init();
        void read(&NMEA_msg msg);
        void send(int PGN, uint_8[] data );
        void send(NMEA_msg msg);
};
