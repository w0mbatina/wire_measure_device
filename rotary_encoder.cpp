#include "rotary_encoder.h"

int encoder_direction() {
  
   encoder.tick();

   RotaryEncoder::Direction smer = encoder.getDirection();


   return int(smer);
   

}
