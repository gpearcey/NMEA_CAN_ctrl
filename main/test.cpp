
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "twaiCanController.h"
#include "driver/gpio.h"
#include "NMEA_msg.h"

extern "C" void app_main(void)
{
    
    twaiCANController twai1;

    twai1.init(GPIO_NUM_34, GPIO_NUM_32);
    printf("finished init");


    //for (int i = 0; i < 100; i++){
    //    NMEA_msg msg;
    //    msg.PGN = 127488;
    //    msg.src = 0;
    //    msg.priority = 3;
    //    msg.length = 8;
    //    msg.data[0] = 0xDD;
    //    msg.data[1] = 0xDD;
    //    msg.data[2] = 0xDD;
    //    msg.data[3] = 0xEE;
    //    msg.data[4] = 0x45;
    //    msg.data[5] = 0xEE;
    //    msg.data[6] = 0x34;
    //    msg.data[7] = 0x45;
    //    twai1.send(msg);
    //    printf("sending message number %d \n", i);
    //}
    
    //for (int i = 0; i < 100; i++){
    //    NMEA_msg rx_msg;
    //    twai1.read(rx_msg);
    //    printf("PGN: %u\n", rx_msg.PGN);
    //    printf("Data: %u\n", rx_msg.data[4]);
    //    printf("\n");    
    //}
    //NMEA_msg rx_msg;
    //twai1.read(rx_msg);
    //printf("PGN: %u\n", rx_msg.PGN);
    //printf("Data: %u\n", rx_msg.data[4]);
    //printf("\n");
//
    twai1.deinit();

    printf("finished deinit");


}
