#include "temp_sensor.h"
#include <Wire.h>


const uint8_t temp_address = 0b0011000;
const uint8_t temp_resultReg = 0x05;

float temp_read() {
  Wire.beginTransmission(temp_address);
  Wire.write(temp_resultReg);
  uint8_t error = Wire.endTransmission();

  if (error != 0) {
    Serial.print("Transmission error in temp sensor temp_read:");
    Serial.println(error);
    return NAN;
  }

  uint8_t requested_byte_amount = Wire.requestFrom(temp_address, 2);  //to zato da lahko in the future preverim koliko jih je
  if (requested_byte_amount != 2) {
    Serial.println("Temperature sensor: incomplete read");
    return NAN;
  }


  uint8_t byte_high = Wire.read();
  uint8_t byte_low = Wire.read();

  uint16_t raw_temp = (uint16_t(byte_high) << 8) | byte_low;  //shifting da bits

  float temperature = (raw_temp & 0x0FFF) * 0.0625f; //tukaj izračunam dejansko temperaturo

  if ((raw_temp & 0x1000) != 0){     //check 12. bita če je nastavljen na 1. Če je, potem je temperatura negativna, in zaradi nekih weird ass jajc kako ta čip računa, je treba odšteti od rezultata 256 stopinj da dobimo dejansko negativno temperaturo
    temperature = temperature - 256.0f; //f dodajamo zato da je navaden float, ne double. Če je double porabi 2x več spomina (64 namesto 32 bitov)
  }

  return temperature;
}