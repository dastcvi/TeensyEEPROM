# TeensyEEPROM

This libary defines a robust system for storing configurations in Teensy EEPROM

## Design Goals

1. *Robust to EEPROM failure*: configurations can still be accessed and updated from some hard-coded default even if EEPROM fails (they'll just reset on reboot). Further, the system should be aware of EEPROM failure.
2. *Versioned*: easy to update configuration versions and determine in the program if the version is wrong
3. *Simple interface*: only one line should be needed to read/write a configuration value