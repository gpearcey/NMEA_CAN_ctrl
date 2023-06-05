#ifndef NMEA_MSG_H
#define NMEA_MSG_H

#include <string>

struct NMEA_msg {
    uint32_t PGN : 18;
    uint8_t src;
    std::string name;
    uint8_t priority : 3;
    uint8_t length : 4;
    int numFields;
    uint8_t data[8];
};

#endif