// soundform no.3, main controller
// natura machina

#define NUM_MODULES 16

const int MAX_TIME_ON  = 15000; //15 sec (frames disable heating after 10s)
const int TIME_BETWEEN_MODES = 1000;

int   totalModes = 5;
long  modeDuration = 0; //initial
int   mode = 0;//initialization 

bool freezeMode = false; //freezes on single mode selected above

// animation modes
// 1 : figure 8 pattern
// 2 : chase
// 3 : (not implemented)
// 4 : groups
// 5 : random picks then lighting

//duration of modes
const long  m1_duration = 30000; //chase
const long  m2_duration = 0; //60000; //sequence
const long  m3_duration = 0; //skip over
const long  m4_duration = 180000;
const long  m5_duration = 180000;

long modeStartTime = 0;

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
  
  /*index: 0 - [ H4 ] {note: } - pin#:*/ 2,
  /*index: 1 - [ G5 ] {note: } - pin#:*/ 3,
  /*index: 2 - [ F4 ] {note: } - pin#:*/ 4,
  /*index: 3 - [ E3 ] {note: } - pin#:*/ 5,
  /*index: 4 - [ D2 ] {note: } - pin#:*/ 6,
  /*index: 5 - [ C1 ] {note: } - pin#:*/ 7,
  /*index: 6 - [ B0 ] {note: } - pin#:*/ 8,
  /*index: 7 - [ A1 ] {note: } - pin#:*/ 9,
  /*index: 8 - [ A4 ] {note: } - pin#:*/ 10,
  /*index: 9 - [ B5 ] {note: } - pin#:*/ 11,
  /*index: 10 -[ C4 ] {note: } - pin#:*/ 12,
  /*index: 11 -[ D3 ] {note: } - pin#:*/ 13,
  /*index: 12 -[ E2 ] {note: } - pin#:*/ A0,
  /*index: 13 -[ F1 ] {note: } - pin#:*/ A1,
  /*index: 14 -[ G0 ] {note: } - pin#:*/ A2,
  /*index: 15 -[ H1 ] {note: } - pin#:*/ A3
 };

 #define x4_groups_total 9 //IMPORTANT: set to length of groups
 
 int x4_groups[x4_groups_total][4] = {
  {},
  {/*A1*/ 7, /*C1*/ 5, /*E3*/   3, /*G5*/ 1}, //bridge (notes C,E,G,A)
  
  {/*A4*/ 8, /*C4*/ 10, /*E2*/ 12, /*G0*/ 14}, //bridge (notes C,E,G,A)
  
  { 3 /*E3*/,   2 /*F4*/,   1 /*G5*/ ,  0 /*H4*/ }, // zone 1  (notes: C, E, G, A)
  { /*D2*/4 ,  /*E3*/ 3,  /*B0*/ 6,  /*B5*/ 9 }, // zone 2
   
  { /*E2*/12 ,  /*D3*/ 11,  /*G0*/ 14, /*G5*/ }, // (notes: C, C, A, A)
  { 12 /*E2*/,  13 /*F1*/,  14 /*G0*/ , 15 /*H1*/ }, // zone 2  (notes: C, E, G, A)
 
 
  { 8 /*A4*/,   9 /*B5*/,   11 /*D3*/,  14 /*E2*/ }, // (notes: C, E, A, A)
  
  {  7 /*A1*/,   8 /*A4*/,   15 /*H1*/ ,  0 /*H4*/ },  // lows

//NOT USED

//  {  8 /*A4*/,   9 /*B5*/,   10 /*C4*/,  11 /*D3*/ },  // zone 4
//  {   5 /*C1*/ ,  4 /*D2*/ ,  3 /*E3*/ ,  2 /*F4*/ }, // diagonal, exclude low (NW->SE)
//  {  10 /*C4*/ , 11 /*D3*/ , 12 /*E2*/ , 13 /*F1*/ }, // diagonal, exclude low (SW->NE)  
//  {-1, -1, -1, -1 },
//  {  12 /*E2*/,  13 /*F1*/,  14 /*G0*/ , 15 /*H1*/ }, // zone 2
//  {   4 /*D2*/,   11 /*D3*/,   12 /*E2*/ ,  3 /*E3*/ }, // highs
//  {   10 /*C4*/,   5 /*C1*/,   13 /*F1*/ ,  2 /*F4*/ }, // mid-highs
//  {   6 /*B0*/,   9 /*B5*/,   1 /*G5*/ ,  14 /*G0*/ },  // mids
//  {-1, -1, -1, -1 },
//  {   7 /*A1*/,   8 /*A4*/,   2 /*F4*/ ,  13 /*F1*/ },  // lows and mids
//  {-1, -1, -1, -1 },
//  {   7 /*A1*/,   8 /*A4*/,   1 /*G5*/ ,  14 /*G0*/ },  // lows and low-mids
//  {-1, -1, -1, -1 },
//  {-1, -1, -1, -1 },
//  { /*A4*/ 8,   /*F1*/ 13,   /*E2*/ 12,  /*E3*/ 3 },  // special group test
 };


//mode 1 settings
int  m1_stepInc = -1; //winds up starting at 0
long m1_lastStepTime = 0;
int  m1_waitAfterStep = 600;
int  m1_countSteps = 0;
#define NUM_CHASE_ITEMS 16
int  m1_chase_items[NUM_CHASE_ITEMS] = {
  // top level 
  3  /*E3*/ ,
  12 /*E2*/ ,
  4  /*D2*/ ,    
  11 /*D3*/ ,
  // next level down
  2  /*F4*/,   
  13 /*F1*/,     
  5  /*C1*/,   
  10 /*C4*/,   
  // next level down
  1  /*G5*/,   
  14 /*G0*/,     
  6  /*B0*/,   
  9  /*B5*/, 
  // next level down
  0  /*H4*/,   
  15 /*H1*/,     
  7  /*A1*/,   
  8  /*A4*/,
};

//mode 2 settings
int  m2_stepInc = -1; //winds up starting at 0
long m2_lastStepTime = 0;
int  m2_waitAfterStep = 5000;

//mode 4 settings
int  m4_stepInc = -1; //winds up starting at 0
long m4_lastStepTime = 0;
int  m4_waitAfterStep = 10000; //must be more that 10s (only 4 at once)

//mode 5 settings (random picks, then leave on for lightning)
int  m5_stepInc = 0;
long m5_lastStepTime = 0;
int  m5_waitAfterStep = 4500;

bool m5_random_picks[NUM_MODULES] = { 
  false, false, false, false, 
  false, false, false, false, 
  false, false, false, false, 
  false, false, false, false,
};

void setup() {
  Serial.begin(115200);
  delay(100);
  
  //initialize pins  
  for(int i = 0; i < NUM_MODULES; i++) {
    pinMode(relay_pins[i], OUTPUT);
    digitalWrite(relay_pins[i], HIGH);
  }

  //instantiate modules
  for(int i = 0; i < NUM_MODULES; i++) {
    modules[i] = new Module(relay_pins[i], i);
  }

  //starting in mode 1
  modeStartTime = millis();

  Serial.println(modeDuration);
}

void loop() {


    switch(mode) {

      case 1: //mode no.1 (CHASE)

          if(m1_countSteps<NUM_CHASE_ITEMS) {
           
            if(millis() - m1_lastStepTime > m1_waitAfterStep) {
              m1_lastStepTime = millis();//record time
  
              m1_stepInc++; //increment to next module
              int mod_index = m1_stepInc % NUM_CHASE_ITEMS;
              modules[m1_chase_items[mod_index]]->turnOn(); 
              
              m1_countSteps++;
          }
        }
          
       case 2: //mode 2 : sequence, figure 8

//          if(millis() - m2_lastStepTime > m2_waitAfterStep) {
//            m2_lastStepTime = millis();//record time
//            m2_stepInc++; //increment to next module
//            int mod_index = m2_stepInc % NUM_MODULES;
//            modules[mod_index]->turnOn(); //modules automatically turn off with module[x]->update()
//          }

        break;

       case 3: //mode 3 = 3x groups

        // TODO
        
        break;

       case 4: //mode 4 = 4x groups

          if(millis() - m4_lastStepTime > m4_waitAfterStep) {
            m4_lastStepTime = millis();//record time
  
            m4_stepInc++; //increment to next module
            int group_index = m4_stepInc % x4_groups_total;

            Serial.println("-----------------");
            Serial.print("turning on group #");
            Serial.println(group_index);
 
            for(int i = 0; i < 4; i++) {
              if(x4_groups[group_index][i] != -1) {
                modules[ x4_groups[group_index][i] ]->turnOff(); 
                delay(100);
                modules[ x4_groups[group_index][i] ]->turnOn(); 
              } else {
                Serial.println(".");
              }
            }
          }
          break;

        // TODO

       case 5: //mode 5 = random picks

       if(millis() - m5_lastStepTime > m5_waitAfterStep) {
            m5_lastStepTime = millis();//record tim
            if(getTotalOn() < NUM_MODULES) {
              int rand_index = 0;
              Serial.println("picking random index");
              int count = 0;
              do {
                count++;
                Serial.println(count);
                rand_index = random(NUM_MODULES);
                Serial.println(m5_random_picks[rand_index]);
                Serial.print("total on = ");
                Serial.println(getTotalOn());
                
              } while( m5_random_picks[rand_index] == true && count < 100);
   
              if(!m5_random_picks[rand_index]) {
                 m5_random_picks[rand_index] = true; //record pick
                 modules[rand_index]->leaveOn(); //modules automatically turn off with module[x]->update()
              } 
            }  
        }
        break;
     }

  //update all modules (always run this last)
  for(int i = 0; i < NUM_MODULES; i++) {
    modules[i]->update();
  }
  if(!freezeMode) {
    checkModeChange(); 
  }
  
}

void checkModeChange() {

//  Serial.println(millis() - modeStartTime);
//  Serial.println(t);
  if(millis() - modeStartTime > modeDuration) {

    mode++;
    if(mode > totalModes) mode = 1;
    
    Serial.print("Switching to mode ");
    Serial.println(mode);
    Serial.print("Delay ");
    Serial.println(TIME_BETWEEN_MODES);
    delay(TIME_BETWEEN_MODES);
    
    
   
    modeStartTime = millis();
     
    switch(mode) {
      
      case 1: //chase intro
      
        Serial.println("-----------------");
        for(int i = 0; i < NUM_MODULES; i++) {
          modules[i]->turnOff();
        }
        
        delay(10000);
        Serial.println("-----------------");
        
        modeDuration = m1_duration;
        m1_countSteps = 0;
        break;
        
      case 2:
        modeDuration = m2_duration;
        break;
        
      case 3:
        modeDuration = m3_duration;
        break;
        
      case 4:
        modeDuration = m4_duration;
        break;
        
      case 5:
         for(int i = 0; i < NUM_MODULES; i++) {
          delay(10);
          modules[i]->turnOff();
        }
       //reset random picks
        for(int i = 0; i < NUM_MODULES; i++) {
          m5_random_picks[i] = false;
        }

        modeDuration = m5_duration;
        
        break;
    }
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
