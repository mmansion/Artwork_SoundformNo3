// soundform no.3, main controller
// natura machina


#define NUM_MODULES 16

const int MAX_TIME_ON  = 15000; //15 sec (frames disable heating after 10s)
const int MIN_TIME_OFF = 30000; //30 sec
const int MAX_TOGETHER_ON = 4; //no more than 4 on simultaneously

// animation modes
// 1 : figure 8 pattern
// 2 : groups of 2
// 3 : groups of 3
// 4 : groups of 4
// 5 : random picks
// 6 : lightshow (no sound)


int  mode = 1;

//mode 1 settings
int  m1_stepInc = 0;
long m1_lastStepTime = 0;
int  m1_waitAfterStep = 5000;
int  m1_duration = 60000;

int m4_stepInc = 0;

/**************************************************************
  Positino Map

        A    B    C    D         E    F    G    H
        
       ---------------------------------------------
  0    [  ] [B0] [  ] [  ]      [  ] [  ] [G0] [  ]
       ---------------------------------------------
  1    [A1] [  ] [C1] [  ]      [  ] [F1] [  ] [H1]
       ---------------------------------------------
  2    [  ] [  ] [  ] [D2]      [E2] [  ] [  ] [  ]
       ---------------------------------------------
                                                      <- Cafe Ent
       ---------------------------------------------
  3    [  ] [  ] [  ] [D3]      [E3] [  ] [  ] [  ]
       ---------------------------------------------
  4    [A4] [  ] [C4] [  ]      [  ] [F4] [  ] [H4]
       ---------------------------------------------
  5    [  ] [B5] [  ] [  ]      [  ] [  ] [G5] [  ]

        
*************************************************************/
#include "Module.h";
Module *modules[NUM_MODULES];

int relay_pins[NUM_MODULES] = { 
  
  // figure 8 route:
  
  /*index: 0 - [ H4 ] - pin#:*/ 2,
  /*index: 1 - [ G5 ] - pin#:*/ 3,
  /*index: 2 - [ F4 ] - pin#:*/ 4,
  /*index: 3 - [ E3 ] - pin#:*/ 5,
  /*index: 4 - [ D2 ] - pin#:*/ 6,
  /*index: 5 - [ C1 ] - pin#:*/ 7,
  /*index: 6 - [ B0 ] - pin#:*/ 8,
  /*index: 7 - [ A1 ] - pin#:*/ 9,
  /*index: 8 - [ A4 ] - pin#:*/ 10,
  /*index: 9 - [ B5 ] - pin#:*/ 11,
  /*index: 10 -[ C4 ] - pin#:*/ 12,
  /*index: 11 -[ D3 ] - pin#:*/ 13,
  /*index: 12 -[ E2 ] - pin#:*/ A0,
  /*index: 13 -[ F1 ] - pin#:*/ A1,
  /*index: 14 -[ G0 ] - pin#:*/ A2,
  /*index: 15 -[ H1 ] - pin#:*/ A3
 };

 int x2_groups[4][2] = { // TODO
  { -1 /**/, -1 /**/}, 
  { -1 /**/, -1 /**/}, 
  { -1 /**/, -1 /**/},  
  { -1 /**/, -1 /**/}, 
 };

 int x3_groups[4][3] = { // TODO
  { -1 /**/, -1 /**/, -1 /**/},
  { -1 /**/, -1 /**/, -1 /**/},
  { -1 /**/, -1 /**/, -1 /**/},
  { -1 /**/, -1 /**/, -1 /**/}
 };

 int x4_groups[9][4] = {
  
  // ON: 15s
  {  3 /*E3*/,  2 /*F4*/,  1 /*G5*/,  0 /*H4*/ }, // zone 1
  
  // ON: 15s
  {  7 /*A1*/,  6 /*B0*/,  5 /*C1*/,  4 /*D2*/ }, // zone 3

  // REST: 15s
  { -1 ,  -1 , -1 , -1 }, //off

  // ON: 15s
  { 12 /*E2*/, 13 /*F1*/, 14 /*G0*/, 15 /*H1*/ }, // zone 2

  // ON: 15s
  {  8 /*A4*/,  9 /*B5*/, 10 /*C4*/, 11 /*D3*/ }, // zone 4

  // REST: 15s
  { -1 ,  -1 , -1 , -1 }, //off

  // ON: 15s
  { 5  /*C1*/ ,  4 /*D2*/ ,  3 /*E3*/ ,  2 /*F4*/ }, // diagonal, exclude low (NW->SE)

  // ON: 15s
  { 10 /*C4*/ , 11 /*D3*/ , 12 /*E2*/ , 13 /*F1*/ }, // diagonal, exclude low (SW->NE)

  // REST: 15s
  { -1 ,  -1 , -1 , -1 }, //off
  
 };

void setup() {
  Serial.begin(115200);
  
  //initialize pins  
  for(int i = 0; i < NUM_MODULES; i++) {
    pinMode(relay_pins[i], OUTPUT);
    digitalWrite(relay_pins[i], HIGH);
  }

  //instantiate modules
  for(int i = 0; i < NUM_MODULES; i++) {
    modules[i] = new Module(relay_pins[i]);
  }
}

void loop() {

  //cannot proceed if running too many at once
  if(getTotalOn() <= MAX_TOGETHER_ON) { 

    switch(mode) {

      case 1: //mode no.1
      
          if(millis() - m1_lastStepTime > m1_waitAfterStep) {
            m1_lastStepTime = millis();//record time
  
            m1_stepInc++; //increment to next module
            int mod_index = m1_stepInc % NUM_MODULES;
            
            Serial.print("turning on module #");
            Serial.println(mod_index);
            
            modules[mod_index]->turnOn(); //modules automatically turn off with module[x]->update()
          }
          break;
          
       case 2: //mode 2 = 2x groups

        // TODO
        
        break;

       case 3: //mode 3 = 3x groups

        // TODO
        
        break;

       case 4: //mode 4 = 4x groups

        // TODO

       case 5: //mode 5 = lightning mode
        
        break;
    }
  }

  //update all modules (always run this last)
  for(int i = 0; i < NUM_MODULES; i++) {
    modules[i]->update();
  }
}

int getTotalOn() {
  int t = 0;
  for(int i = 0; i < NUM_MODULES; i++) {
    if(modules[i]->isOn()) t++;
  }
  return t;
}

void changeMode(int newMode) {
  mode = newMode;
}
