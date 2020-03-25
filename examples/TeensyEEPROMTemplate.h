/*
 *  TeensyEEPROMTemplate.h
 *  Author:  Alex St. Clair
 *  Created: March 2020
 *
 *  A template showing how to use TeensyEEPROM
 */

#ifndef TEENSYEEPROMTEMPLATE_H
#define TEENSYEEPROMTEMPLATE_H

#include "TeensyEEPROM.h"

struct example_struct_t {
    float f1;
    uint8_t u[3];
};

class TeensyEEPROMTemplate : public TeensyEEPROM {
public:
    TeensyEEPROMTemplate();

    // constants, manually change VERSION to force update (both MUST be static)
    static const uint16_t EEPROM_VERSION = 0x5C03;
    static const uint16_t BASE_ADDR = 0x0000;

    // data for EEPROM (create EEPROMData object here for each)
    EEPROMData<example_struct_t> s0;
    EEPROMData<float> f0;
    EEPROMData<int32_t> i0;
    EEPROMData<uint8_t> u0;

private:
    void RegisterAll();

};

#endif /* TEENSYEEPROMTEMPLATE_H */