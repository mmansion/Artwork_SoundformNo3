// soundform no.3, main controller
// natura machina


#define NUM_MODULES 16

const int MAX_TIME_ON  = 10000; //10 sec
const int MIN_TIME_OFF = 30000; //30 sec
const int MAX_TOGETHER_ON = 4; //no more than 4 on simultaneously

// animation modes
// 1 : step through relay_pins in order (base route)
// 2 : todo, add more modes
int  mode = 1;

//mode 1 settings
int  m1_stepInc = 0;
long m1_lastStepTime = 0;
int  m1_waitAfterStep = 5000;

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

int relay_pins[NUM_MODULES] = { //base route
  2,  // 0,  Pin D2   = Pos H4
  3,  // 1,  Pin D3   = Pos G5
  4,  // 2,  Pin D4   = Pos F4
  5,  // 3,  Pin D5   = Pos E3
  6,  // 4,  Pin D6   = Pos D2
  7,  // 5,  Pin D7   = Pos C1
  8,  // 6,  Pin D8   = Pos B0
  9,  // 7,  Pin D9   = Pos A1
  10, // 8,  Pin D10  = Pos A4
  11, // 9,  Pin D11  = Pos B5
  12, // 10, Pin D12  = Pos C4
  13, // 11, Pin D13  = Pos D3
  A0, // 12, Pin A0   = Pos E2
  A1, // 13, Pin A1   = Pos F1
  A2, // 14, Pin A2   = Pos G0
  A3  // 15, Pin A3   = Pos H1
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
          
       case 2: //mode no.2

        // TODO
        
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
