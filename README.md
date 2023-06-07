# NMEA_CAN_ctrl
ESP-IDF Project to transmit and receive NMEA messages to and from multiple connection points with an ESP32 module.
## Getting Started
Follow [this](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) guide to install ESP_IDF. 
### Hardware
This project is set up to use 2 TWAI controllers, and 1 MCP2515 for CAN controllers, and 3 CAN transceivers. Different CAN controllers can be used by implementing the CAN_Controller class for the desired controller. 
