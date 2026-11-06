#include <Arduino.h>
#include <math.h>
#include <limits.h>  //to smo vključili zato, ker rabimo testirat ali je rezultat večji od maksimalne int številke
#include "ads.h"
#include "menu_data.h"

extern float copper_resistivity;

float sense_current(float sense_voltage);
float wire_crossection(int wire);  //tle bomo vlekli iz menuWireMetricSetValue iz menu_data
float wire_resistance(float current, float voltage_drop);
int wire_length(float copper, float wire_resistance, float wire_cross);
int wire_calculator (int wire_size, float wire_voltage, float sense_voltage);
int wire_averageLength (int wire_size);