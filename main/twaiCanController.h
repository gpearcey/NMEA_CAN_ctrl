#include "CAN_controller.h"
#include "esp32_twai.h"
#include "NMEA_msg.h"
#include "driver/gpio.h"

class twaiCANController : public class CANController
{
    public:
        void init(gpio_num_t rxPin, gpio_num_t txPin);
        void read(&NMEA_msg msg);
        void send(int PGN, uint_8[] data );
        void send(NMEA_msg msg);
};
