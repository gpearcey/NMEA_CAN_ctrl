#include "twaiCanController.h"


void twaiCANController::init(gpio_num_t rxPin, gpio_num_t txPin){
    this->controller.setCANPins(rxPin, txPin);
    this->controller.begin(250000); //sets baud rate, installs and starts TWAI drivers
    return;
}

void twaiCANController::deinit(){
    this->controller.disable();//stops TWAI drivers and uninstalls them
    return;
}

void twaiCANController::read(NMEA_msg& msg){
    CAN_FRAME rxframe;
    this->controller.read(rxframe);
    msg = CANtoNMEA(rxframe);
    return;
}

void twaiCANController::send(NMEA_msg msg){

    CAN_FRAME frame = NMEAtoCAN(msg);
    controller.sendFrame(frame);
    return;
}

CAN_FRAME twaiCANController::NMEAtoCAN(NMEA_msg msg){
    CAN_FRAME frame;
    frame.rtr = 0;
    
    std::string strID = std::to_string(msg.priority) + std::to_string(msg.PGN) + std::to_string(msg.src);
    frame.id = std::stoi(strID);
    
    frame.extended = true;
    frame.length = 8;
    frame.data.uint8[0] = msg.data[0];
    frame.data.uint8[1] = msg.data[1];
    frame.data.uint8[2] = msg.data[2];
    frame.data.uint8[3] = msg.data[3];
    frame.data.uint8[4] = msg.data[4];
    frame.data.uint8[5] = msg.data[5];
    frame.data.uint8[6] = msg.data[6];
    frame.data.uint8[7] = msg.data[7];

    return frame;

}

NMEA_msg twaiCANController::CANtoNMEA(CAN_FRAME frame){
    NMEA_msg msg;
    
    std::string strID = std::to_string(frame.id);
    std::reverse(strID.begin(), strID.end());
    std::string strSrc = strID.substr(0,8);
    std::string strPGN = strID.substr(8,18);
    std::string strPriority = strID.substr(26,3);

    msg.PGN = std::stoi(strPGN);
    msg.src = std::stoi(strSrc);
    //msg.name TODO
    msg.priority = std::stoi(strPriority);
    msg.length = frame.length;
    //msg.numFeilds TODO
    msg.data[0] = frame.data.uint8[0];
    msg.data[1] = frame.data.uint8[1]; 
    msg.data[2] = frame.data.uint8[2]; 
    msg.data[3] = frame.data.uint8[3]; 
    msg.data[4] = frame.data.uint8[4]; 
    msg.data[5] = frame.data.uint8[5]; 
    msg.data[6] = frame.data.uint8[6]; 
    msg.data[7] = frame.data.uint8[7]; 

    return msg;

}
