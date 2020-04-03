/*
 *  TeensyEEPROM.cpp
 *  Author:  Alex St. Clair
 *  Created: March 2020
 *
 *  This class manages configuration storage in EEPROM on a Teensy
 */

#include "TeensyEEPROM.h"

TeensyEEPROM::TeensyEEPROM(uint16_t version_number, uint16_t start_address)
    : version(version_number)
    , start_addr(start_address)
    , next_addr(start_address + sizeof(version))
{ }

bool TeensyEEPROM::Initialize()
{
    uint16_t eeprom_version = 0;

    // formulate the data structure managing all of the configs
    RegisterAll();

    // read the version number from EEPROM
    EEPROM.get(start_addr, eeprom_version);

    // make sure the version number in EEPROM is correct
    if (version != eeprom_version) {
        Reconfigure();
        return false;
    } else {
        UpdateAll();
        return true;
    }
}

uint16_t TeensyEEPROM::Bufferize(uint8_t * buffer, uint16_t buffer_size)
{
    uint16_t bytes_written = 0;

    // make sure the input buffer is big enough
    if (buffer_size < (next_addr - start_addr)) return 0;

    // iterate through EEPROM and write the contents
    for (uint16_t i = start_addr; i < next_addr; i++) {
        buffer[bytes_written++] = EEPROM.read(i);
    }

    return bytes_written;
}

bool TeensyEEPROM::Register(EEPROMDatatype * data)
{
    // check that it will fit (E2END from avr/eeprom.h)
    if ((next_addr + data->data_length) > (E2END + 1)) {
        return false;
    }

    // set the address for the data point
    data->eeprom_address = start_addr + next_addr;

    // note the increase in size
    next_addr += data->data_length;

    // add to the linked list
    if (NULL == head) {
        head = data;
        tail = data;
    } else {
        tail->next = data;
        tail = data;
    }

    return true;
}

void TeensyEEPROM::UpdateAll()
{
    EEPROMDatatype * node = head;

    // traverse the linked list, updating each node
    while (NULL != node) {
        node->Update();
        node = node->next;
    }
}

void TeensyEEPROM::Reconfigure()
{
    EEPROMDatatype * node = head;

    EEPROM.put(start_addr, version);

    // traverse the linked list, reseting each node
    while (NULL != node) {
        node->Reset();
        node = node->next;
    }
}
