// soundform no.3, frame controller (MODULE)
// copyright - natura machina


const int NUM_LIGHTS = 4; //fixed (don't change)
const int FADE_INCREMENT = 5; //speed of fade
const int MAX_BRIGHTNESS = 200; //0-255 pwm
const int RELAY_MAX_TIME_ON = 10000;// ms
const bool RANDOM_START_POS = true; //pick random side to start led animation

// pin assignments
const int RELAY_PIN = A0;
const int MOSFET_PINS[4] = { 3, 6, 9, 11 };

long anim_start_times[8] {  //time to start each step (ms)
    1000,  // fade in led 0
    2000,  // fade in led 1
    3000,  // fade in led 2
    4000,  // fade in led 3
    10500, // fade out led 0
    11000, // fade out led 1
    11500, // fade out led 2
    12000, // fade out led 3  
};

bool anim_triggered[8] { //check if step complete
    false,  // fade in led 0
    false,  // fade in led 1
    false,  // fade in led 2
    false,  // fade in led 3
    false, // fade out led 0
    false, // fade out led 1
    false, // fade out led 2
    false, // fade out led 3  
};

#include "LedLight.h"
LedLight *leds[4]; //led lights

//------------------------------------------
//params for easeAllIn and easeAllOut

//easing durations (seconds)
float easeAllIn_duration = 5.0;
float easeAllOut_duration = 5.0;

//easing scales
float easeAllIn_scale  = 1.0;
float easeAllOut_scale = 1.0;


void setup() {
  Serial.begin(115200);

  // setup pins
  pinMode(RELAY_PIN, OUTPUT);

  // setup mosfet pins
  for(int i = 0; i < NUM_LIGHTS; i++) {
    pinMode(MOSFET_PINS[i], OUTPUT);
  }

  // initialize led lights
  int offset = (RANDOM_START_POS) ? random(4): 0;
  
  for(int i = 0; i < NUM_LIGHTS; i++) {  
    int pos = (offset + i) % NUM_LIGHTS;
    leds[i] = new LedLight( MOSFET_PINS[pos] );
  }
}

void loop() { 

  //1. control heating elem (OFF/ON)
  
    if(millis() < RELAY_MAX_TIME_ON) {
      digitalWrite(RELAY_PIN, HIGH);
    } else {
      digitalWrite(RELAY_PIN, LOW);
    }

  // 2. animated led lighting
   
    for(int t = 0; t < 8; t++) {

      //trigger animation if time, and if not already triggered
      if(millis() > anim_start_times[t] && !anim_triggered[t]) {
        
        anim_triggered[t] = true;
        
        switch(t) { //select the animation
          
          //animation steps, based on anim_start_times

          //-------------------------------------------------
          // sequence to fade lights in
          
          case 0: //fade in led 0
            leds[0]->setTargetBrightness(MAX_BRIGHTNESS);
            break;
            
          case 1: //fade in led 1
            leds[1]->setTargetBrightness(MAX_BRIGHTNESS);
            break;
            
          case 2: //fade in led 2
            leds[2]->setTargetBrightness(MAX_BRIGHTNESS);
            break;
            
          case 3: //fade in led 3
            leds[3]->setTargetBrightness(MAX_BRIGHTNESS);
            break;

          //-------------------------------------------------
          // sequence to fade lights out
            
          case 4: //fade out led 0
            leds[0]->setTargetBrightness(0);
            break;
            
         case 5:
            leds[1]->setTargetBrightness(0);
            break;
            
         case 6:
            leds[2]->setTargetBrightness(0);
            break;
            
         case 7:
            leds[3]->setTargetBrightness(0);
            break;
        }
      }
    }

    //update all leds
    for(int i = 0; i < NUM_LIGHTS; i++) {
      leds[i]->update();
    }

  delay(10);
}
