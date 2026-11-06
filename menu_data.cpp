#include "menu_data.h"

String menuMeasureOptions[]{
  "Upornost",
  "Dolzina"
};
int menuMeasureValues[]{
  0,  //upornost
  1,  //Dolzina Zice
};
int menuMeasureOptionsSetValue = 0;

String menuWireMetricOptions[] {
    "0,25",
    "0,35",
    "0,50",
    "0,75",
    "1,00"
};
int menuWireMetricValues[]{
  25,   //"0,25",
  35,   //"0,35",
  50,   //"0,50",
  75,   //"0,75",
  100,  //"1,00
};
int menuWireMetricSetValue = 25;


String menuMultiplexOptions[]{ //določen array stringov za meni 1
                               "A0-A1",
                               "A0-A3",
                               "A1-A3",
                               "A2-A3",
                               "A0-GND",
                               "A1-GND",
                               "A2-GND",
                               "A3-GND"
};
int menuMultiplexValues[]{
  0b000,  //A0-A1
  0b001,  //A0-A3
  0b010,  //A1-A3
  0b011,  //A2-A3
  0b100,  //A0-GND
  0b101,  //A1-GND
  0b110,  //A2-GND
  0b111   //A3-GND
};
int menuMultiplexSetValue = 0b000;  //v to vrednost se zapisuje dejanska vrednost, ki jo potem uporabljam v kodi za confige, računanje, itd. Vrednost, ki je zapisana notr je default vrednost ko se program zažene

String menuFsrWireOptions[]{
  "6.144V",
  "4.096V",
  "2.048V",
  "1.024V",
  "0.512V",
  "0.256V"
};
int menuFsrWireValues[]{
  0b000,  //6.144V
  0b001,  //4.096V
  0b010,  //2.048V
  0b011,  //1.024V
  0b100,  //0.512V
  0b101   //0.256V
};
int menuFsrWireSetValue = 0b000;

String menuFsrSenseOptions[]{
  "6.144V",
  "4.096V",
  "2.048V",
  "1.024V",
  "0.512V",
  "0.256V"
};
int menuFsrSenseValues[]{
  0b000,  //6.144V
  0b001,  //4.096V
  0b010,  //2.048V
  0b011,  //1.024V
  0b100,  //0.512V
  0b101   //0.256V
};
int menuFsrSenseSetValue = 0b000;


String menuModeOptions[]{
  "s-s",
  "con."
};
int menuModeValues[]{
  0b1,  //single shot
  0b0   //continous
};
int menuModeSetValue = 0b1;



String menuDataRateOptions[]{
  "8sps",
  "16sps",
  "32sps",
  "64sps",
  "128sps",
  "250sps",
  "475sps",
  "860sps",
};
int menuDataRateValues[]{
  0b000,  //8sps
  0b001,  //16sps
  0b010,  //32sps
  0b011,  //64sps
  0b100,  //128sps
  0b101,  //250sps
  0b110,  //475sps
  0b111,  //860sps
};
int menuDataRateSetValue = 0b100;

Menu menus[]{ //ARRAY MENIJEV, tukaj je vsak array en svoj meni, potem pa ma še en array z options znotraj vsakega!
              {
                "Measure options",
                menuMeasureOptions,
                menuMeasureValues,
                &menuMeasureOptionsSetValue,
                2 },
              { "Wire metric",
                menuWireMetricOptions,
                menuWireMetricValues,
                &menuWireMetricSetValue, 
                5 },                
              { "Multiplex",
                menuMultiplexOptions,
                menuMultiplexValues,
                &menuMultiplexSetValue,  //& se doda zato da deklariram da je to tle naslov, ne dejanska vrednost
                8 },
              { "FSR wire",
                menuFsrWireOptions,
                menuFsrWireValues,
                &menuFsrWireSetValue,
                6 },
              { "FSR sense",
                menuFsrSenseOptions,
                menuFsrSenseValues,
                &menuFsrSenseSetValue,
                6 },
              
              { "Mode",
                menuModeOptions,
                menuModeValues,
                &menuModeSetValue,
                2 },
              { "Data rate",
                menuDataRateOptions,
                menuDataRateValues,
                &menuDataRateSetValue,
                8 }
};

uint8_t menuNo = 7;  //v tej spremenljivki je treba določiti število menijev, uporabno za pol da delamo logiko gumbov, ko pridemo do konca
