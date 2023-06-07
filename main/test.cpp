#include <stdio.h>
#include "twaiCanController.h"
#include "driver/gpio.h"
#include "NMEA_msg.h"
#include "esp_log.h"

static const char* TAG = "test.cpp";

extern "C" void app_main(void)
{
    twaiCANController twai1(GPIO_NUM_34, GPIO_NUM_32);

    twai1.init();

    //Transmit messages
    for (int i = 0; i < 10; i++){
        NMEA_msg msg;
        msg.PGN = 127508;
        msg.src = 3;
        msg.priority = 1;
        msg.length = 8;
        msg.data[0] = 0xDD;
        msg.data[1] = 0xDD;
        msg.data[2] = 0xDD;
        msg.data[3] = 0xEE;
        msg.data[4] = 0xEE;
        msg.data[5] = 0xEE;
        msg.data[6] = 0xEE;
        msg.data[7] = 0xEE;
        twai1.transmit(msg);
    }
    
    //Receive Messages
    for (int i = 0; i < 100; i++){
        NMEA_msg rx_msg;
        twai1.receive(rx_msg);
        ESP_LOGD(TAG,"PGN: %u\n", rx_msg.PGN);
        ESP_LOGD(TAG,"Data: %u\n", rx_msg.data[4]);
        ESP_LOGD(TAG,"Data Length: %u\n", rx_msg.length);
        ESP_LOGD(TAG,"\n");    
    }


    twai1.deinit();
}
