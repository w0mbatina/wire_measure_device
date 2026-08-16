#include "ads.h"
#include <Wire.h>

float fsr_res;

bool ads_config(
  uint8_t selected_multiplex,  //deklarirane spremenljivke, ki jih rabim za uporabo te funkcije, to so zgornje konstante za config
  uint8_t selected_fsr,
  uint8_t selected_op_mode,
  uint8_t selected_data_rate) {

  uint16_t samples_per_second;  //ta se uporabi samo za računanje delaya

  switch (selected_data_rate) {  //switch da določi vrednost "samples_per_second", ki se potem uporabi v računanju delaya za to da se naredi vsaj ena meritev
    case 0b000:
      samples_per_second = 8;
      break;
    case 0b001:
      samples_per_second = 16;
      break;
    case 0b010:
      samples_per_second = 32;
      break;
    case 0b011:
      samples_per_second = 64;
      break;
    case 0b100:
      samples_per_second = 128;
      break;
    case 0b101:
      samples_per_second = 250;
      break;
    case 0b110:
      samples_per_second = 475;
      break;
    case 0b111:
      samples_per_second = 860;
      break;
    default:  //v defaultu pogleda če je katerikola vrednost ki ni specifično pravilna, in potem pove da nekej ni ok
      Serial.println("Invalid ADS1115 data rate");
      return false;
  }

  switch (selected_fsr) {  //ta switch se uporabi namesto if funkcij, ne morem vrjet da je trajal tolk časa da sm to faking odkril
    case 0b000:
      fsr_res = 6.144;
      break;
    case 0b001:
      fsr_res = 4.096;
      break;
    case 0b010:
      fsr_res = 2.048;
      break;
    case 0b011:
      fsr_res = 1.024;
      break;
    case 0b100:
      fsr_res = 0.512;
      break;
    case 0b101:
    case 0b110:
    case 0b111:
      fsr_res = 0.256;
      break;
    default:
      Serial.println("Invalid ADS1115 FSR");
      return false;
  }
  

  //SWITCHE preverimo preden gremo računat in vnašat v ADS, ker drugače lahko zapišemo napačne vrednosti, in šele potem preverimo "ups, ni kul"

  uint16_t config = (0b1 << 15) | (selected_multiplex << 12) | (selected_fsr << 9) | (selected_op_mode << 8) | (selected_data_rate << 5) | 0b00011;

  uint8_t config_high = config >> 8;         //premaknem za 8 v desno, low byte se zbriše ostane samo high
  uint8_t config_low = config & 0b11111111;  //comparam z 00000000 111111111 da mi taprve nule pobrišejo cel zgornji byte, spodnji pa ostane. ker je spremenljivka uint8_t, se vanjo zapiše zamo zadnjih 8 bitov, se pravi samo spodnji byte

  Wire.beginTransmission(ads_address);  //0x48 je naslov, to se lahko določi z povezovanjem enega piina na različne meste, 0x48 je default
  Wire.write(config_register);          //pišem naslov config registr v address pointer register, od tle naprej potem piše v config register, config_register je deklariran čisto na vrhu

  Wire.write(config_high);  //zapišem zgornji byte regirstra
  Wire.write(config_low);   //zapišem spodnji byte config registra

  uint8_t error = Wire.endTransmission();

  if (error != 0) {  //chekc če je prišlo do kakšnega errorja, če pride, potem prekine funkcijo in funkcija returna FALSE
    Serial.print("Transmission error in ads_config: ");
    Serial.println(error);
    return false;
  }

  delay((1000 / samples_per_second) + 5);  //delay, da se opravi vsaj ena meritev

  return true;  //če je ok funkcija returna TRUE
}

bool ads_selectRegister(uint8_t register_address) {  //funkcija za spreminanje naslovnega registra, spremenljivke z pravimi vrednostmi so na vrhu
  Wire.beginTransmission(ads_address);
  Wire.write(register_address);

  uint8_t error = Wire.endTransmission();

  if (error != 0) {  //chekc če je prišlo do kakšnega errorja, če pride, potem prekine funkcijo in funkcija returna FALSE
    Serial.print("Transmission error in ads_selectRegister: ");
    Serial.println(error);
    return false;
  }

  return true;  //če je ok funkcija returna TRUE
}

float ads_readResult() {

  uint8_t received = Wire.requestFrom(ads_address, 2);  //requesta koliko je bytov za poslat zapiše koliko jih je available. Tukaj mora biti številka 2, ker še nisem nobenega prebral.

  if (received != 2) {  //check če je bil slučajno error. Če je bil, vrne NOT A NUMBER in konča funkcijo
    Serial.println("ADS1115 read error");
    return NAN;
  }
  uint8_t byte_high = Wire.read();  //prebere prvega
  uint8_t byte_low = Wire.read();   //prebere drugega

  int16_t raw = (uint16_t(byte_high) << 8) | byte_low;  //pokombinira oba byta v eno 16 bitno vrednost.
  float final_value = raw * (fsr_res / 32768.0f);       //preračuna final value glede na FSR resolucijo.

  return final_value;
}