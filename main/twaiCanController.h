#ifndef TWAI_CAN_CONTROLLER_H
#define TWAI_CAN_CONTROLLER_H

#include <string>
#include "CAN_controller.h"
#include "esp32_twai.h"
#include "NMEA_msg.h"
#include "driver/gpio.h"


class twaiCANController : public CANController
{
    public:
        void init(gpio_num_t rxPin, gpio_num_t txPin) override;
        void deinit() override;
        void read(NMEA_msg& msg) override;
        void send(NMEA_msg msg) override;

    private:
    CAN_FRAME NMEAtoCAN(NMEA_msg msg);
    NMEA_msg CANtoNMEA(CAN_FRAME frame);

    ESP32CAN controller;

};

#endif //TWAI_CAN_CONTROLLER_H