#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "button.h"
#include "menu_data.h"
#include "menu_behaviour.h"
#include "ads.h"
#include "meta.h"
#include "rotary_encoder.h"
#include "wire_math.h"

uint8_t dt = 32;  //definirani pini za encoder
uint8_t clk = 33;
uint8_t rotary_switch = 25;

uint8_t buttonOK = 26;  //definiran pin za OK button

LiquidCrystal_I2C lcd(0x27, 16, 2);                               //definiran LCD display
RotaryEncoder encoder(clk, dt, RotaryEncoder::LatchMode::FOUR3);  //definiran rotary encoder za kontrolo

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
  lcd.print(menus[0].options[selectionFinder(0)]);  // menus[4].options izbere seznam opcij menija 4 (Data rate) selectionFinder(4) poišče indeks trenutno izbrane opcije v istem meniju. Obe številki morata biti enaki, sicer iščemo izbrano opcijo v napačnem meniju in jo uporabimo za prikaz opcije iz menija 4.
  lcd.print("  ");
  lcd.print(menus[1].options[selectionFinder(1)]);
  lcd.setCursor(0, 1);
  lcd.print(menus[3].options[selectionFinder(3)]);
  lcd.print("  ");
  lcd.print(menus[4].options[selectionFinder(4)]);
}


void setup() {

  //pinMode(clk, INPUT_PULLUP); clk in dt ni treba definirat za rotary encoder, ker to naredi library sam
  pinMode(rotary_switch, INPUT_PULLUP);
  //pinMode(dt, INPUT_PULLUP);
  pinMode(buttonOK, INPUT_PULLUP);

  Serial.begin(115200);
  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(DEVICE_NAME);
  lcd.setCursor(0, 1);
  lcd.print("FW: ");
  lcd.print(FIRMWARE_VERSION);
}

void loop() {

  int smer = encoder_direction();

  if (!menuOpen) {  //obnašanje gumbov v primeru, da meniji niso odprti. Definirati je treba še za gumb za merjenje
    if (smer != 0) {
      menuOpen = true;
      subMenu = false;
      mainIndex = 0;
      subIndex = selectionFinder(mainIndex);
      lcd_printMenu();
      return;
    }

    if (buttonPress(rotary_switch)) {
      lcd_printSettings();
      return;
    }

    if (buttonPress(buttonOK)) {

      if (menuMeasureOptionsSetValue == 0) {
        ADS_doubleResult voltages = ads_doubleRead(menuFsrWireSetValue, menuFsrSenseSetValue, menuModeSetValue, menuDataRateSetValue);  //ta struct je definiran v ads.h

        float voltage_1 = voltages.A0_A1;  //takole se kliče vrednosti iz structa
        float voltage_2 = voltages.A2_A3;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("A0_A1: ");
        lcd.print(voltage_1, 6);
        lcd.setCursor(0, 1);
        lcd.print("A2_A3: ");
        lcd.print(voltage_2, 6);
      }
      else {
        int dolzina = wire_averageLength(menuWireMetricSetValue);

        //float current_zatest = sense_current(sense_voltage) * 1000;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Dolzina zice [m]");
        lcd.setCursor(0, 1);
        lcd.print(dolzina);
        lcd.print("m  ");
        //lcd.print(current_zatest);
        //lcd.print("mA");
      }
    }
  }

  else {             //obnašanje gumbov, če je meni odprt
    if (!subMenu) {  //tukaj je meni odprt, submenu pa še niso odprti, zato skrolamo direkt samo po glavnih menijih
      if (smer == 1) {
        mainIndex = indexNext(mainIndex, menuNo);
        subIndex = selectionFinder(mainIndex);
        lcd_printMenu();
        return;
      }

      if (smer == -1) {
        mainIndex = indexPrevious(mainIndex, menuNo);
        subIndex = selectionFinder(mainIndex);
        lcd_printMenu();
        return;
      }

      if (buttonPress(rotary_switch)) {
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
      if (smer == 1) {
        subIndex = indexNext(subIndex, menus[mainIndex].optionCount);
        lcd_printMenu();
        return;
      }

      if (smer == -1) {
        subIndex = indexPrevious(subIndex, menus[mainIndex].optionCount);
        lcd_printMenu();
        return;
      }

      if (buttonPress(rotary_switch)) {
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
