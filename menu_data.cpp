#include "menu_data.h"


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
  0b000,   //A0-A1
  0b001,   //A0-A3
  0b010,   //A1-A3
  0b011,   //A2-A3
  0b100,  //A0-GND
  0b101,  //A1-GND
  0b110,  //A2-GND
  0b111  //A3-GND
};
int menuMultiplexSetValue = 0b000; //v to vrednost se zapisuje dejanska vrednost, ki jo potem uporabljam v kodi za confige, računanje, itd. Vrednost, ki je zapisana notr je default vrednost ko se program zažene

String menuFsrOptions[]{
"6.144V",
"4.096V",
"2.048V",
"1.024V",
"0.512V",
"0.256V"
};
int menuFsrValues[]{
0b000,    //6.144V
0b001,    //4.096V
0b010,    //2.048V
0b011,    //1.024V
0b100,    //0.512V
0b101    //0.256V
};
int menuFsrSetValue = 0b000;


String menuModeOptions[]{
  "s-s",
  "con."
};
int menuModeValues[]{
  0b1,   //single shot
  0b0  //continous
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
0b000,    //8sps
0b001,   //16sps
0b010,   //32sps
0b011,   //64sps
0b100,  //128sps
0b101,  //250sps
0b110,  //475sps
0b111,  //860sps
};
int menuDataRateSetValue = 0b100;

Menu menus[]{ //ARRAY MENIJEV, tukaj je vsak array en svoj meni, potem pa ma še en array z options znotraj vsakega!
              {
                "Multiplex",
                menuMultiplexOptions,
                menuMultiplexValues,
                &menuMultiplexSetValue, //& se doda zato da deklariram da je to tle naslov, ne dejanska vrednost
                8 },
              { "FSR",
                menuFsrOptions,
                menuFsrValues,
                &menuFsrSetValue,
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

uint8_t menuNo = 4;  //v tej spremenljivki je treba določiti število menijev, uporabno za pol da delamo logiko gumbov, ko pridemo do konca
