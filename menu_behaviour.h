#ifndef MENU_BEHAVIOUR_H
#define MENU_BEHAVIOUR_H

#include <Arduino.h>

extern uint8_t mainIndex;
extern uint8_t subIndex;
extern bool menuOpen;  //gledam ali je meni odprt
extern bool subMenu;   //gledam ali skrolam po podmeniju ali ne

uint8_t indexNext (uint8_t index, uint8_t refrence);
uint8_t indexPrevious (uint8_t index, uint8_t refrence);
uint8_t selectionFinder(uint8_t currentMainIndex);

#endif