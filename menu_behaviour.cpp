#include "menu_behaviour.h"
#include "menu_data.h"

uint8_t mainIndex = 0;
uint8_t subIndex = 0;
bool menuOpen = false;  //gledam ali je meni odprt
bool subMenu = false;   //gledam ali skrolam po podmeniju ali ne

uint8_t indexNext (uint8_t index, uint8_t reference){
  if (index == (reference - 1)) {
    index = 0;
    } 
  else {
    index++;
    }
  return index;
}

uint8_t indexPrevious (uint8_t index, uint8_t reference){
  if (index < 1) {
    index = reference - 1;
    } 
  else {
    index--;
    }
  return index;
}

uint8_t selectionFinder(uint8_t currentMainIndex){
  uint8_t i = 0;
  while (i < menus[currentMainIndex].optionCount) {
      if (menus[currentMainIndex].values[i] == *menus[currentMainIndex].setValue){
        return i;
      }
      i++;
    }
  return 0;
}