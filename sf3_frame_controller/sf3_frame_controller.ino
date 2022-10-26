// soundform no.3, frame controller (MODULE)
// copyright - natura machina


const int NUM_LIGHTS = 4; //fixed (don't change)
const int FADE_SPEED = 5; //speed of fade
const int MAX_BRIGHTNESS = 200; //0-255 pwm
const int RELAY_MAX_TIME_ON = 10000;// ms
const bool RANDOM_START_POS = false; //pick random side to start led animation

const int RANDOM_LIGHTNING_ODDS = 100; //1 in # odds
const int LIGHTNING_BRIGHTNESS = 80; //0-255 (less than max)
const int LIGHTNING_FADE_SPEED = 10; //speed of fade in LIGHTNING mode

// pin assignments
const int RELAY_PIN = A0;
const int MOSFET_PINS[4] = { 3, 6, 9, 11 };

long anim_start_times[9] {  //time to start each step (ms)
    1000,  // fade in led 0
    2000,  // fade in led 1
    3000,  // fade in led 2
    4000,  // fade in led 3
    10500, // fade out led 0
    11000, // fade out led 1
    11500, // fade out led 2
    12000, // fade out led 3 
    60000, // LIGHTNING mode
};

bool anim_triggered[9] { //check if step complete
    false,  // fade in led 0
    false,  // fade in led 1
    false,  // fade in led 2
    false,  // fade in led 3
    false, // fade out led 0
    false, // fade out led 1
    false, // fade out led 2
    false, // fade out led 3 
    false, // LIGHTNING mode 
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
bool notified_on = false;
bool notified_off = false;
void setup() {
  Serial.begin(115200);

  // setup pins
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);//verify off

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
      if(!notified_on) {
        Serial.println("Relay ON");
        notified_on = true;
      }
      digitalWrite(RELAY_PIN, HIGH);
    } else {
      if(!notified_off) {
        Serial.println("Relay OFF");
        notified_off = true;
      }
      digitalWrite(RELAY_PIN, LOW);
    }

  // 2. animated led lighting
   
    for(int t = 0; t < 9; t++) {

      //trigger animation if time, and if not already triggered
      if(millis() > anim_start_times[t] && !anim_triggered[t]) {
        
        anim_triggered[t] = true;
        
        switch(t) { //select the animation
          
          //animation steps, based on anim_start_times

          //-------------------------------------------------
          // sequence to fade lights in
          
          case 0: //fade in led 0
            Serial.println("Fade In 0");
            leds[0]->setTargetBrightness(MAX_BRIGHTNESS);
            break;
            
          case 1: //fade in led 1
            Serial.println("Fade In 1");
            leds[1]->setTargetBrightness(MAX_BRIGHTNESS);
            break;
            
          case 2: //fade in led 2
            Serial.println("Fade In 2");
            leds[2]->setTargetBrightness(MAX_BRIGHTNESS);
            break;
            
          case 3: //fade in led 3
            Serial.println("Fade In 3");
            leds[3]->setTargetBrightness(MAX_BRIGHTNESS);
            break;

          //-------------------------------------------------
          // sequence to fade lights out
            
          case 4: //fade out led 0
            Serial.println("Fade Out 0");
            leds[0]->setTargetBrightness(0);
            break;
            
         case 5:
            Serial.println("Fade Out 1");
            leds[1]->setTargetBrightness(0);
            break;
            
         case 6:
            Serial.println("Fade Out 2");
            leds[2]->setTargetBrightness(0);
            break;
            
         case 7:
            Serial.println("Fade Out 3");
            leds[3]->setTargetBrightness(0);
            break;

         //-------------------------------------------------
         // LIGHTNING mode
            
         case 8:
            Serial.println("LIGHTNING Mode");
            leds[0]->setLightningMode(true);
            leds[1]->setLightningMode(true);
            leds[2]->setLightningMode(true);
            leds[3]->setLightningMode(true);
            break;
            
          default:
            break;
        }
      }
    }

    //update all leds
    for(int i = 0; i < NUM_LIGHTS; i++) {
      leds[i]->update();
    }
}
