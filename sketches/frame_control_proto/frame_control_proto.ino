// soundform no.3, frame controller (MODULE)
// copyright - natura machina


const int NUM_LIGHTS = 4;
const int FADE_STEP = 5;
const int MAX_BRIGHTNESS = 200;

const int RELAY_PIN = A0;
const int RELAY_MAX_TIME_ON = 10000;//ms

long anim_step_times[8] {  //time to start each step (ms)
    1000,  // fade in led 1
    2000,  // fade in led 2
    3000,  // fade in led 3
    4000,  // fade in led 4
    10500, // fade out led 1
    11000, // fade out led 2
    11500, // fade out led 3
    12000, // fade out led 4  
};

bool anim_checklist[8] { //check if step complete
    false,  // fade in led 1
    false,  // fade in led 2
    false,  // fade in led 3
    false,  // fade in led 4
    false, // fade out led 1
    false, // fade out led 2
    false, // fade out led 3
    false, // fade out led 4  
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


//------------------------------------------
#include <Easing.h>

// pin assignments



const int mosfet_pins[4] = { 3, 6, 9, 11 };
int       mosfet_pwms[4] = { 0, 0, 0, 0  };



void setup() {
  Serial.begin(115200);

  // setup pins
  pinMode(RELAY_PIN, OUTPUT);

  // setup mosfet pins
  for(int i = 0; i < NUM_LIGHTS; i++) {
    pinMode(mosfet_pins[i], OUTPUT);
  }

  //randomize pin order

  // initialie led lights
  for(int i = 0; i < NUM_LIGHTS; i++) {
    
    leds = new LedLight(mosfet_pins[i]);
  }

  
}

void loop() { 

  //1. control relay (heating element)
  
    if(millis() < RELAY_MAX_TIME_ON) {
      digitalWrite(RELAY_PIN, HIGH);
    } else {
      digitalWrite(RELAY_PIN, LOW);
    }

  // 2. control led lighting
   
    for(int t = 0; t < 8; t++) {
      if(millis()
    }

    //update all leds
    for(int i = 0; i < NUM_LIGHTS; i++) {
      leds[i]->update();
    }

  


  delay(10);
}


//void easeAllIn(int duration, int scale) { //blocking
//  
//  // options: Back, Bounce, Circ, Cubic, 
//  // Elastic, Expo, Linear, Quad, Quart, Quint, Sine
//  
//  EasingFunc<Ease::Linear> easing;
//  
//  easing.duration(easeAllIn_duration);
//  easing.scale(easeAllIn_scale);
//
//  float start_sec = millis() / 1000;
//  float now = millis() / 1000;
//  
//  while (now <= start_sec + duration) {
//    float perc = easing.get(now - start_sec);
//    float brightness = MAX_BRIGHTNESS * perc;
//    now = millis() / 1000;
//  }
//  delay(50);
//}
//
//void easeAllOut(int duration, int scale) { //blocking
//  
//  // options: Back, Bounce, Circ, Cubic, 
//  // Elastic, Expo, Linear, Quad, Quart, Quint, Sine
//  
//  EasingFunc<Ease::Linear> easing;
//  
//  easing.duration(easeAllOut_duration);
//  easing.scale(easeAllOut_scale);
//
//  float start_sec = millis() / 1000;
//  float now = millis() / 1000;
//  
//  while (now <= start_sec + duration) {
//    float perc = easing.get(now - start_sec);
//    float brightness = MAX_BRIGHTNESS - (MAX_BRIGHTNESS * perc);
//    now = millis() / 1000;
//  }
//  delay(50);
//}


// int curr_index = 0;
//int curr_brightness = 0;
//
//
//bool dir = 1;
//float inc       = 0.001;
//float start_pos = 0.0;
//float end_pos   = 1.0;
//float perc      = 0.0;
//float angle     = PI*2;
//bool topspeed = false;
//long timestamp_topspeed = 0;
//int  remain_at_topspeed = 3000;
//int x = 0;
//int last_x = 0;

//  x = 100 - (sin( angle ) * 100);
//
//  if(topspeed) {
//    if(millis() - timestamp_topspeed > remain_at_topspeed) {
//      angle = 0.0;
//      topspeed = false;
//    }
//    x = 100;
//    doStep();
//  } else if(x > 100) {
//    topspeed = true;
//    timestamp_topspeed = millis();
//    x = 100;
//  } else {
//    angle+=inc;
//    if(abs(x-last_x) > 0) { doStep();}
//    last_x = x;
//  }
