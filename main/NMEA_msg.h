#ifndef NMEA_MSG_H
#define NMEA_MSG_H

#include <string>

struct NMEA_msg {
    uint32_t PGN : 18;
    uint8_t src;
    std::string name;
    uint8_t priority : 3;
    int length;
    int numFields;
    uint8_t data[223];
};

#endif //NMEA_MSG_H