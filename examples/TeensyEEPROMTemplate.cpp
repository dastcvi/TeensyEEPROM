/*
 *  TeensyEEPROMTemplate.cpp
 *  Author:  Alex St. Clair
 *  Created: March 2020
 *
 *  A template showing how to use TeensyEEPROM
 */

#include "TeensyEEPROMTemplate.h"

TeensyEEPROMTemplate::TeensyEEPROMTemplate()
    : TeensyEEPROM(EEPROM_VERSION, BASE_ADDR)
    // all data objects are set to default values here
    , s0({1.0f, {2,3,4}})
    , f0(100.0f)
    , i0(100000)
    , u0(100)
{ }

void TeensyEEPROMTemplate::RegisterAll()
{
    bool success = true;

    // call register for each object
    success &= Register(&s0);
    success &= Register(&f0);
    success &= Register(&i0);
    success &= Register(&u0);

    if (!success) {
        Serial.println("Error registering EEPROM data points");
    }
}