#ifndef MENU_DATA_H
#define MENU_DATA_H

#include <Arduino.h>

extern uint8_t menuNo; 

struct Menu {  //struct za menije. Vsak meni vsebuje naslov in opcije.
  String title;
  String* options;  // * je pointer, in kaže na spodnji array ki sem ga definiral za vsak meni. Narejen je pointer, ker če bi tukaj definiral array, moram določiti točno število vrednosti, k ijih vsebuje. Če pa samo pointam, pa tega ni treba, ker array ni shranjen v structu, ampak je definiran drugje, pointer pa samo kaže pot do njega
  int* values; // dejanske vrednosti teh options, ki so v submenijih
  int* setValue;
  uint8_t optionCount;  // koliko možnosti imam v meniju, to se bo uporabljalo zato, da lahko pravilno določam kaj se zgodi, ko skrolam gor in dol (in verjetno še za dejansko funkcionalnost)
};

extern String menuMultiplexOptions[];
extern int menuMultiplexValues[];
extern int menuMultiplexSetValue;

extern String menuFsrOptions[];
extern int menuFsrValues[];
extern int menuFsrSetValue;

extern String menuModeOptions[];
extern int menuModeValues[];
extern int menuModeSetValue;

extern String menuDataRateOptions[];
extern int menuDataRateValues[];
extern int menuDataRateSetValue;

extern Menu menus[];

#endif