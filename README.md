# TeensyEEPROM

## Overview

This libary defines a robust system for storing configurations in Teensy EEPROM that extends the core [EEPROM library](https://www.arduino.cc/en/Reference/EEPROM). Each configuration value is duplicated in RAM and has a hard-coded default stored in FLASH so that in case of complete EEPROM failure, the interface can still be used to access and update configurations (though they'll be restored to their defaults on a processor reset). Additionally, the library maintains a configuration version number. This way, if the microcontroller resets and the incorrect version is detected (in case of a new program or EEPROM failure), it can restore the default configuration values. Finally, the library maintains all addressing for the user and provides one-line read and write methods.

## Library Contents

`TeensyEEPROM.h/cpp`: the library code

`examples/TeensyEEPROMTemplate.h/cpp`: a template that shows how to use the class

`examples/TeensyEEPROM_Test.ino`: a test Arduino script using the template

## Implementation

*Understanding the implemenation is not necessary for using the library, but this explanation is included for those who are curious.* The implementation is an abstract base class, `TeensyEEPROM`, that serves as the interface to the EEPROM, maintaining a version number and all data points and their addresses. The library provides the `EEPROMData` type from which all data points must be instantiated. `EEPROMData` is templated to allow users complete flexibility in their configuration types. `EEPROMData` objects maintain FLASH defaults, the RAM duplicates, and metadata used by the `TeensyEEPROM` class for addressing. Each `EEPROMData` object must be registered with the `TeensyEEPROM` class on initialization, which is enforced by the inclusion of a pure virtual protected method `RegisterAll()` called by the public `Initialize()` method. The `EEPROMData` class inherits from the abstract base class `EEPROMDatatype` that allows `TeensyEEPROM` to access the necessary metadata as a friend class, and to allow the provision of a protected `Register(EEPROMDatatype *)` method for intended use in `RegisterAll()`.

## Usage

### Creating an Inheriting Class

The user should create a general configuration class that inherits from `TeensyEEPROM` and instantiates `EEPROMData` objects for each configuration datapoint. A complete example is included in the `examples/` directory and explained here. The class definition could be modeled after the following class:

```C++
class TeensyEEPROMTemplate : public TeensyEEPROM {
public:
    TeensyEEPROMTemplate();

    // constants, manually change VERSION to force update (both MUST be static)
    static const uint16_t EEPROM_VERSION = 0x5C03;
    static const uint16_t BASE_ADDR = 0x0000;

    // data for EEPROM with specified types (create EEPROMData object here for each)
    EEPROMData<example_struct_t> s0;
    EEPROMData<float> f0;
    EEPROMData<int32_t> i0;
    EEPROMData<uint8_t> u0;

private:
    // must be implemented, and must register each EEPROMData object
    void RegisterAll();

};
```

The `EEPROM_VERSION` and `BASE_ADDR` variables are passed to the base class in the constructor (note that the **must** be static), and all of the `EEPROMData` configuration defaults are also hard-coded in the constructor:

```C++
TeensyEEPROMTemplate::TeensyEEPROMTemplate()
    : TeensyEEPROM(EEPROM_VERSION, BASE_ADDR)
    , s0({1.0f, {2,3,4}})
    , f0(100.0f)
    , i0(100000)
    , u0(100)
{ }
```

The only other requirement is that the user must implement the pure virtual `RegisterAll()` method from `TeensyEEPROM`, which could look as follows:

```C++
void TeensyEEPROMTemplate::RegisterAll()
{
    bool success = true;

    // call register for each object
    // (will only return false if there's not enough space left in EEPROM)
    success &= Register(&s0);
    success &= Register(&f0);
    success &= Register(&i0);
    success &= Register(&u0);

    if (!success) {
        Serial.println("Error registering EEPROM data points");
    }
}
```

### Using the Class

With the class set up, usage is easy. First, the user must instantiate an object of the inheriting class:

```C++
TeensyEEPROMTemplate eeprom_template;
```

On initialization of the software after reboot, the user **must** call the `Initialize()` method inherited from `TeensyEEPROM`, which will return false if the version number in EEPROM doesn't match the one in FLASH:

```C++
if (!eeprom_template.Initialize()) {
    Serial.println("Error initializing from EEPROM, reconfigured to default");
}
```

With this complete, the user can read and write configurations defined in the inheriting class at will:

```C++
// read example, returns whatever type the object is
eeprom_template.f0.Read();

// read only one element of an array in a struct
eeprom_template.s0.Read().u[2];

// write examples (you must write an entire struct)
eeprom_template.f0.Write(3.1415);
eeprom_template.s0.Write(new_struct_s0);
```

### Versioning

Whenever the user updates the number/type/order of configurations in EEPROM, they **must manually** update the EEPROM_VERSION or equivalent variable/macro passed into the `TeensyEEPROM` constructor. This will force the class to reconfigure the EEPROM.

### Future Work

* *Improved Versioning*: it would be best if the user didn't have to manually update the version each time the configuration structure changes
* *Checksum/CRC*: right now, the only way an EEPROM failure can be detected is if the version number in EEPROM doesn't match the version number in FLASH. It would be possible to add a checksum or CRC to the `TeensyEEPROM` class that will automatically update every time a configuration is changed. The downside to this is that it will increase EEPROM write cycles, and will increase execution time whenever a configuration is changed.