#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "button.h"
#include "menu_data.h"
#include "menu_behaviour.h"
#include "ads.h"
#include "meta.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

uint8_t buttonL = 32;  //pač definirani pini za gumbe
uint8_t buttonM = 33;
uint8_t buttonR = 25;
uint8_t buttonOK = 26;



void lcd_printMenu() {
  if (!subMenu) {
    lcd.clear();
    lcd.print("->");
    lcd.print(menus[mainIndex].title);
    lcd.setCursor(0, 1);
    lcd.print("  ");
    lcd.print(menus[mainIndex].options[subIndex]);
  } else {
    lcd.clear();
    lcd.print("  ");
    lcd.print(menus[mainIndex].title);
    lcd.setCursor(0, 1);
    lcd.print("->");
    lcd.print(menus[mainIndex].options[subIndex]);
  }
}

void lcd_printSettings() {
  lcd.clear();
  lcd.print(menus[0].options[selectionFinder(0)]);
  lcd.print("  ");
  lcd.print(menus[3].options[selectionFinder(3)]);
  lcd.setCursor(0,1);
  lcd.print(menus[1].options[selectionFinder(1)]);
  lcd.print("  ");
  lcd.print(menus[2].options[selectionFinder(2)]);
}


void setup() {

  pinMode(buttonL, INPUT_PULLUP);
  pinMode(buttonM, INPUT_PULLUP);
  pinMode(buttonR, INPUT_PULLUP);
  pinMode(buttonOK, INPUT_PULLUP);

  Serial.begin(115200);
  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(DEVICE_NAME);
  lcd.setCursor(0,1);
  lcd.print("FW: ");
  lcd.print(FIRMWARE_VERSION);
}

void loop() {

  if (!menuOpen) {  //obnašanje gumbov v primeru, da meniji niso odprti. Definirati je treba še za gumb za merjenje
    if (buttonPress(buttonR) || buttonPress(buttonL)) {
      menuOpen = true;
      subMenu = false;
      mainIndex = 0;
      subIndex = selectionFinder(mainIndex);
      lcd_printMenu();
      return;
    }

    if (buttonPress(buttonM)) {
      lcd_printSettings();
      return;
    }

    if (buttonPress(buttonOK)) {
      if (!ads_config(
            menuMultiplexSetValue,
            menuFsrSetValue,
            menuModeSetValue,
            menuDataRateSetValue)) {  //kliče config funkcijo za single shot meritev in preveri glede na returnan boolean če je sploh uspelo. Če ni uspelo, prekine loop() in se vrne na začetek

        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ads_config error");

        return;
      }
      if (!ads_selectRegister(conversion_register)) {  //kliče config funkcijo za adress register naslov in preveri glede na returnan boolean če je sploh uspelo. Če ni uspelo, prekine loop() in se vrne na začetek
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("select register error");

        return;
      }

      float voltage = ads_readResult();

      if (!isnan(voltage)) {
        Serial.println(voltage, 6);
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Voltage: ");
      lcd.setCursor(0, 1);
      lcd.print(voltage, 6);
    }
  }

  else {             //obnašanje gumbov, če je meni odprt
    if (!subMenu) {  //tukaj je meni odprt, submenu pa še niso odprti, zato skrolamo direkt samo po glavnih menijih
      if (buttonPress(buttonR)) {
        mainIndex = indexNext(mainIndex, menuNo);
        subIndex = selectionFinder(mainIndex);
        lcd_printMenu();
        return;
      }

      if (buttonPress(buttonL)) {
        mainIndex = indexPrevious(mainIndex, menuNo);
        subIndex = selectionFinder(mainIndex);
        lcd_printMenu();
        return;
      }

      if (buttonPress(buttonM)) {
        subMenu = true;
        subIndex = selectionFinder(mainIndex);
        lcd_printMenu();
        return;
      }

      if (buttonPress(buttonOK)) {
        menuOpen = false;
        lcd_printSettings();

      }
    }

    else {  //tukaj pa nato skrolamo znotraj submenijev
      if (buttonPress(buttonR)) {
        subIndex = indexNext(subIndex, menus[mainIndex].optionCount);
        lcd_printMenu();
        return;
      }

      if (buttonPress(buttonL)) {
        subIndex = indexPrevious(subIndex, menus[mainIndex].optionCount);
        lcd_printMenu();
        return;
      }

      if (buttonPress(buttonM)) {
        menuOpen = false;
        subMenu = false;
        *menus[mainIndex].setValue = menus[mainIndex].values[subIndex];  //// * dereferencira pointer: grem na naslov, ki je shranjen v setValue, in v dejansko spremenljivko zapišem izbrano vrednost
        mainIndex = 0;
        subIndex = 0;
        lcd.clear();
        lcd_printSettings();
        return;
      }

      if (buttonPress(buttonOK)) {
        subMenu = false;
        subIndex = selectionFinder(mainIndex);
        lcd_printMenu();
      }
    }
  }
}
