#include "twaiCanController.h"

static const char* TAG = "twaiCanController";

twaiCANController::twaiCANController(gpio_num_t txPin, gpio_num_t rxPin) : g_config(TWAI_GENERAL_CONFIG_DEFAULT(txPin, rxPin, TWAI_MODE_NORMAL)){}

void twaiCANController::init(){
   
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

void twaiCANController::receive(NMEA_msg& msg){
    //Wait for message to be received
    twai_message_t message;
    if (twai_receive(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        ESP_LOGI(TAG,"Message received\n");
    } else {
        ESP_LOGI(TAG,"Failed to receive message\n");
        return;
    }
    //convert message to NMEA format
    msg = CANtoNMEA(message);
    return;
}

void twaiCANController::transmit(NMEA_msg msg){
    //convert message to twai (CAN) format
    twai_message_t message = NMEAtoCAN(msg);
    if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        ESP_LOGI(TAG,"Message queued for transmission\n");
    } else {
        ESP_LOGI(TAG,"Failed to queue message for transmission\n");
    }
    return;
}

twai_message_t twaiCANController::NMEAtoCAN(NMEA_msg msg){
    twai_message_t t_msg;

    //set remote transmission request to 0 to send standard data frame
    t_msg.rtr = 0;    

    //create CAN ID
    std::string strPriority = std::bitset<3>(msg.priority).to_string();
    std::string strPGN = std::bitset<18>(msg.PGN).to_string();
    std::string strSrc = std::bitset<8>(msg.src).to_string();
    std::string strID = strPriority + strPGN + strSrc;    
    t_msg.identifier = std::stoi(strID, nullptr, 2);

    ESP_LOGD(TAG, "Message ID Created: %lx \n", t_msg.identifier);

    //all NMEA messages have extended 29 bt ID
    t_msg.extd = true;

    //message length in bytes
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
    
        ESP_LOGD(TAG,"ID to be converted: %lx \n",t_msg.identifier);

        //parse the ID to get priority, PGN and source
        std::string strID = std::bitset<32>(t_msg.identifier).to_string();    
        std::string strSrc = strID.substr(24,8);
        std::string strPGN = strID.substr(6,18);
        std::string strPriority = strID.substr(3,3);

        msg.PGN = std::stoi(strPGN, nullptr, 2);
        msg.src = std::stoi(strSrc, nullptr, 2);
        msg.priority = std::stoi(strPriority);
        msg.length = t_msg.data_length_code;

        msg.data[0] = t_msg.data[0];
        msg.data[1] = t_msg.data[1]; 
        msg.data[2] = t_msg.data[2]; 
        msg.data[3] = t_msg.data[3]; 
        msg.data[4] = t_msg.data[4]; 
        msg.data[5] = t_msg.data[5]; 
        msg.data[6] = t_msg.data[6]; 
        msg.data[7] = t_msg.data[7];

    }
    else{
        ESP_LOGI(TAG, "Incomming message is not in NMEA format");
    }
    return msg;
}
