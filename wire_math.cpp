#include "wire_math.h"
#include <math.h>    //to smo vključili ker imamo noter neko round funkcijo
#include <limits.h>  //to smo vključili zato, ker rabimo testirat ali je rezultat večji od maksimalne int številke
#include "ads.h"
#include "menu_data.h"

float sense_resistor = 10.0f;          //f zato da določimo da je spremenjljivka sigurno float, in ne int. Določimo glede na to kakšen je dejanski fizični upor v beadboardu
float copper_resistivity = 1.68e-8f;  //1.68e-8f je pravilno, če dam 1.68 * 10e-8f to pomeni 10x10^-8, kar je za 10x narobe. Lahko se napiše tudi 1.68 * 1e-8f


float sense_current(float sense_voltage) {
  float current = sense_voltage / sense_resistor;

  if (!isfinite(current) || current < 0) {  //isfinite preveri več stvari kot isnan, zato to namesto isnan
    Serial.println("Nekje se je zataknilo pri racunanju toka");
    return NAN;
  }

  if (current == 0.0f) {
    Serial.println("Tok je 0.0A, nekej ni ok");
    return NAN;
  }

  return current;
}

float wire_crossection(int wire) {  //tle bomo vlekli iz menuWireMetricSetValue iz menu_data

  if (wire <= 0.0f) {
    Serial.println("Neveljaven presek zice");
    return NAN;
  }

  float wire_cross = wire * 1e-8f;

  return wire_cross;
}

float wire_resistance(float current, float voltage_drop) {
  float resistance = voltage_drop / current;

  if (!isfinite(resistance) || resistance < 0.0f) {
    Serial.println("Zataknilo se je pri racunanju upornosti zice");
    return NAN;
  }

  return resistance;
}

int wire_length(float copper, float wire_resistance, float wire_cross) {  //tukaj še preverimo ali je manjše od 0 in ali je večje od max integerja ki je lahko.
  float length = (wire_cross * wire_resistance) / copper;

  if (!isfinite(length) || length < 0 || length >= (float)INT_MAX) {
    Serial.println("Zataknilo se je pri racunanju dolzine");
    return -1;
  }

  int int_length = (int)round(length);

  return int_length;
}

int wire_calculator (int wire_size, float wire_voltage, float sense_voltage){

  float wire_cross = wire_crossection(wire_size);
  float current = sense_current(sense_voltage);
  float wire_resistance_calc = wire_resistance(current, wire_voltage);
  float length = wire_length(copper_resistivity, wire_resistance_calc, wire_cross);

  if (length < 0 || length >= INT_MAX){
    Serial.println("Problem pri končni kalkulaciji v wire_calculator");
    return -1;
  }

  return length;

}

int wire_averageLength (int wire_size) {

  float wire_samples[5];
  float sense_samples[5];

  for (int i = 0; i<5; i++){
    ADS_doubleResult voltages = ads_doubleRead(menuFsrWireSetValue, menuFsrSenseSetValue, menuModeSetValue, menuDataRateSetValue);

    wire_samples[i] = voltages.A0_A1;
    sense_samples[i] = voltages.A2_A3;
    
  }
 
  float wire_average = wire_samples[0];
  float sense_average = sense_samples[0];

  for (int i = 1; i < 5; i++){
    wire_average = wire_average + wire_samples[i];
    sense_average = sense_average + sense_samples[i];
  }

  wire_average = wire_average / 5;
  sense_average = sense_average / 5;

  int dolzina = wire_calculator (wire_size, wire_average, sense_average);

  return dolzina;
}