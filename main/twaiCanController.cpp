#include "twaiCanController.h"


void twaiCANController::init(gpio_num_t rxPin, gpio_num_t txPin){
    ESP32CAN controller(rxPin, txPin);
    return;
}

void twaiCANController::read(NMEA_msg& msg){
    CAN_FRAME rxframe;
    controller.read(&rxframe);

    msg.pgn = 
    return;
}

void twaiCANController::send(NMEA_msg msg){
    CAN_FRAME txframe;
    txframe.rtr = 
    tx
}