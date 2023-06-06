#include "twaiCanController.h"

static const char* TAG = "twaiCanController";


void twaiCANController::init(gpio_num_t txPin, gpio_num_t rxPin){
    
   
    //Install TWAI driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        ESP_LOGI(TAG, "Driver installed\n");
    } else {
        ESP_LOGI(TAG, "Failed to install driver\n");
        return;
    }

    //Start TWAI driver
    if (twai_start() == ESP_OK) {
        ESP_LOGI(TAG, "Driver started\n");
    } else {
        ESP_LOGI(TAG,"Failed to start driver\n");
    }

    return;
}

void twaiCANController::deinit(){
    //Stop the TWAI driver
    if (twai_stop() == ESP_OK) {
    ESP_LOGI(TAG, "Driver stopped\n");
    } else {
        ESP_LOGI(TAG,"Failed to stop driver\n");
        return;
    }

    //Uninstall the TWAI driver
    if (twai_driver_uninstall() == ESP_OK) {
        ESP_LOGI(TAG,"Driver uninstalled\n");
    } else {
        ESP_LOGI(TAG,"Failed to uninstall driver\n");
    }
    return;
}

void twaiCANController::read(NMEA_msg& msg){
    //Wait for message to be received
    twai_message_t message;
    if (twai_receive(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        ESP_LOGI(TAG,"Message received\n");
    } else {
        ESP_LOGI(TAG,"Failed to receive message\n");
        return;
    }

        //Process received message
    //if (message.extd) {
    //    printf("Message is in Extended Format\n");
    //} else {
    //    printf("Message is in Standard Format\n");
    //}
    //printf("ID is %lu\n", message.identifier);
    //if (!(message.rtr)) {
    //    for (int i = 0; i < message.data_length_code; i++) {
    //        printf("Data byte %d = %d\n", i, message.data[i]);
    //    }
    //}
    msg = CANtoNMEA(message);
    return;
}

void twaiCANController::send(NMEA_msg msg){

    twai_message_t message;//     = NMEAtoCAN(msg);
    message.identifier = 0x8F80103;
    message.extd = 1;
    message.data_length_code = 8;
    for (int i = 0; i < 8; i++) {
        message.data[i] = 20;
    }
    //Queue message for transmission
    if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        ESP_LOGI(TAG,"Message queued for transmission\n");
    } else {
        ESP_LOGI(TAG,"Failed to queue message for transmission\n");
    }
    return;
}

twai_message_t twaiCANController::NMEAtoCAN(NMEA_msg msg){
    twai_message_t t_msg;
    t_msg.rtr = 0;
    
    std::string strID = std::to_string(msg.priority) + std::to_string(msg.PGN) + std::to_string(msg.src);
    t_msg.identifier = std::stoi(strID);
    
    t_msg.extd = true;
    t_msg.data_length_code = 8;
    t_msg.data[0] = msg.data[0];
    t_msg.data[1] = msg.data[1];
    t_msg.data[2] = msg.data[2];
    t_msg.data[3] = msg.data[3];
    t_msg.data[4] = msg.data[4];
    t_msg.data[5] = msg.data[5];
    t_msg.data[6] = msg.data[6];
    t_msg.data[7] = msg.data[7];

    return t_msg;

}

NMEA_msg twaiCANController::CANtoNMEA(twai_message_t t_msg){
    NMEA_msg msg;
    if (!(t_msg.rtr) && (t_msg.extd)){
        
    
        std::string strID = std::bitset<32>(t_msg.identifier).to_string();
        printf("ID: %lx \n",t_msg.identifier);
        
        //std::reverse(strID.begin(), strID.end());
        std::string strSrc = strID.substr(24,8);
        std::string strPGN = strID.substr(6,18);
        std::string strPriority = strID.substr(3,3);

        msg.PGN = std::stoi(strPGN, nullptr, 2);
        msg.src = std::stoi(strSrc, nullptr, 2);
        //msg.name TODO
        msg.priority = std::stoi(strPriority);
        msg.length = t_msg.data_length_code;
        //msg.numFeilds TODO
        msg.data[0] = t_msg.data[0];
        msg.data[1] = t_msg.data[1]; 
        msg.data[2] = t_msg.data[2]; 
        msg.data[3] = t_msg.data[3]; 
        msg.data[4] = t_msg.data[4]; 
        msg.data[5] = t_msg.data[5]; 
        msg.data[6] = t_msg.data[6]; 
        msg.data[7] = t_msg.data[7];

        return msg;

    }
    else{
        ESP_LOGI(TAG, "Incomming message is not in NMEA format");
        return msg;
    }

}
