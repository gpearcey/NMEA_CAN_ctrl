#ifndef TWAI_CAN_CONTROLLER_H
#define TWAI_CAN_CONTROLLER_H

#include <string>
#include <bitset>
#include "CAN_controller.h"
#include "esp_log.h"
#include "driver/twai.h"

class twaiCANController : public CANController
{
    public:
        void init(gpio_num_t txPin, gpio_num_t rxPin) override;
        void deinit() override;
        void receive(NMEA_msg& msg) override;
        void transmit(NMEA_msg msg) override;

    private:
        twai_message_t NMEAtoCAN(NMEA_msg msg);
        NMEA_msg CANtoNMEA(twai_message_t t_msg);

        //set timing to 250kbits for NMEA
        static constexpr twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS();
        //Filter for incoming messages
        static constexpr twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
        //Configure TX and RX pins, set twai mode
        static constexpr twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_32, GPIO_NUM_34, TWAI_MODE_NORMAL);


};

#endif //TWAI_CAN_CONTROLLER_H