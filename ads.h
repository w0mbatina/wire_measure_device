#ifndef ADS_H
#define ADS_H

#include <Arduino.h>

const uint8_t ads_address = 0x48;

const uint8_t conversion_register = 0b00;
const uint8_t config_register = 0b01;
const uint8_t Lo_thresh_register = 0b10;
const uint8_t Hi_thresh_register = 0b11;


bool ads_config(
  uint8_t selected_multiplex,  //deklarirane spremenljivke, ki jih rabim za uporabo te funkcije, to so zgornje konstante za config
  uint8_t selected_fsr,
  uint8_t selected_op_mode,
  uint8_t selected_data_rate);

bool ads_selectRegister(uint8_t register_address);

float ads_readResult();


#endif