#ifndef CAN_CONTROLLLER_H
#define CAN_CONTROLLER_H

#include "NMEA_msg.h"
#include "driver/gpio.h"
/**
 * Abstract CAN Controller 
 * Implemented for each specific CAN controller used
*/
class CANController
{
    public:
        virtual ~CANController() {}
        virtual void init(gpio_num_t rxPin, gpio_num_t txPin) = 0;
        virtual void deinit() = 0;
        virtual void read(NMEA_msg& msg) = 0;
        virtual void send(NMEA_msg msg) = 0;
};

#endif // CAN_CONTROLLER_H
