/*
 *  TeensyEEPROM.h
 *  Author:  Alex St. Clair
 *  Created: March 2020
 *
 *  This class manages configuration storage in EEPROM on a Teensy
 */

#ifndef TEENSYEEPROM_H
#define TEENSYEEPROM_H


#include "WProgram.h"
#include <EEPROM.h>
#include <StdInt.h>


// --------------------------------------------------------
// Abstract base class for EEPROMData template class
// --------------------------------------------------------
class EEPROMDatatype {
protected:
    // read from EEPROM or reset to default
    virtual void Update() = 0;
    virtual void Reset() = 0;

    // required for locating the data in EEPROM
    uint16_t eeprom_address = 0;
    uint8_t data_length = 0;

    // for positioning in a linked list
    EEPROMDatatype * next = NULL;

    // allow access for linked list maintenance and EEPROM update/reset
    friend class TeensyEEPROM;
};


// --------------------------------------------------------
// Template class allows for different datatypes
// --------------------------------------------------------
template <typename T>
class EEPROMData : public EEPROMDatatype {
public:
    // constructor: sets both the current and default values
    EEPROMData(const T set_value = T())
    {
        current_value = set_value;
        default_value = set_value;
        data_length = sizeof(T);
    }

    // access methods
    T Read() { return current_value; }

    void Write(T write_value)
    {
        current_value = write_value;
        EEPROM.put(eeprom_address, write_value);
    }

private:
    // read from EEPROM or reset to default
    void Update() { EEPROM.get(eeprom_address, current_value); }
    void Reset() { Write(default_value); }

    // maintain the current and default value
    T current_value;
    T default_value;
};


// --------------------------------------------------------
// Abstract main base class to maintain the EEPROM and data
// --------------------------------------------------------
class TeensyEEPROM {
public:
    TeensyEEPROM();

    bool Initialize();

protected:
    bool Reconfigure();

    bool Register(EEPROMDatatype * data);

    volatile void RegisterAll() = 0;

    EEPROMDatatype * head = NULL;
};


#endif /* TEENSYEEPROM_H */