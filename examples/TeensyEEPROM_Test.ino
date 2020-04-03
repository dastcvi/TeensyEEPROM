/*  TeensyEEPROM_Test.ino
 *  Author: Alex St. Clair
 *  Created: March 2020
 */

#include "TeensyEEPROMTemplate.h"

TeensyEEPROMTemplate eeprom_template;
uint8_t output_buffer[50];
uint16_t buffer_length = 0;

void setup()
{
  Serial.begin(115200);
  while(!Serial);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  if (!eeprom_template.Initialize()) {
    Serial.println("Error initializing from EEPROM, reconfigured to default");
  }

  Serial.print("EEPROM Version: 0x"); Serial.println(eeprom_template.EEPROM_VERSION, HEX);
  Serial.print("EEPROM Base: 0x"); Serial.println(eeprom_template.BASE_ADDR, HEX);
  Serial.println();

  print_all();
}

void print_all()
{
  Serial.print("s0: ");
  Serial.print(eeprom_template.s0.Read().f1);
  Serial.print(", [");
  Serial.print(eeprom_template.s0.Read().u[0]);
  Serial.print(",");
  Serial.print(eeprom_template.s0.Read().u[1]);
  Serial.print(",");
  Serial.print(eeprom_template.s0.Read().u[2]);
  Serial.println("]");

  Serial.print("f0: ");
  Serial.println(eeprom_template.f0.Read());

  Serial.print("i0: ");
  Serial.println(eeprom_template.i0.Read());

  Serial.print("u0: ");
  Serial.println(eeprom_template.u0.Read());
}

void loop()
{
  static float new_f0 = 0.0f;
  static int32_t new_i0 = 0;
  static uint8_t new_u0 = 0;
  static example_struct_t new_s0 = {0};

  // simple CLI:
  //   * to re-print:  "p"
  //   * to change f0: "f,<new float>"
  //   * to change i0: "i,<new int32>"
  //   * to change u0: "u,<new uint8>"
  //   * to change s0: "s,<new float>,<new uint8>,<new uint8>,<new uint8>"
  while (Serial.available()) {
    switch (Serial.read()) {
    case 'p':
      print_all();
      break;
    case 'f':
      new_f0 = Serial.parseFloat(0);
      eeprom_template.f0.Write(new_f0);
      Serial.println("Wrote new f0");
      break;
    case 'i':
      new_i0 = Serial.parseInt(0);
      eeprom_template.i0.Write(new_i0);
      Serial.println("Wrote new i0");
      break;
    case 'u':
      new_u0 = Serial.parseInt(0);
      eeprom_template.u0.Write(new_u0);
      Serial.println("Wrote new u0");
      break;
    case 's':
      new_s0.f1 = Serial.parseFloat(0);
      new_s0.u[0] = Serial.parseInt(0);
      new_s0.u[1] = Serial.parseInt(0);
      new_s0.u[2] = Serial.parseInt(0);
      eeprom_template.s0.Write(new_s0);
      Serial.println("Wrote new s0");
      break;
    case 'b':
      buffer_length = eeprom_template.Bufferize(output_buffer, 50);
      Serial.print("Buffer (len=");
      Serial.print(buffer_length);
      Serial.println("):");
      for (int i = 0; i < buffer_length; i++) {
        Serial.print("0x");
        Serial.print(output_buffer[i], HEX);
        Serial.print(", ");
      }
      Serial.println();
      break;
    default:
      // ignore it
      break;
    }
  }
}
