//------------------------
//ZONE1
const int PIN_H4 = 2;
const int PIN_G5 = 3;
const int PIN_F4 = 4;
const int PIN_E3 = 5;


//------------------------
//ZONE2

const int PIN_H1 = A3;
const int PIN_G0 = A2;
const int PIN_F1 = A1;
const int PIN_E2 = A0;

//------------------------
//ZONE3

const int PIN_A_1 = 9;
const int PIN_B0 = 8;
const int PIN_C1 = 7;
const int PIN_D2 = 6;

//------------------------
//ZONE4

const int PIN_A_4 = 10;
const int PIN_B5 = 11;
const int PIN_C4 = 12;
const int PIN_D3 = 13;


//--------------------------------------------------------
void setup() { 

  //------------------------
  //ZONE1
  pinMode(PIN_H4, OUTPUT);
  pinMode(PIN_G5, OUTPUT);
  pinMode(PIN_F4, OUTPUT);
  pinMode(PIN_E3, OUTPUT);
  digitalWrite(PIN_H4, HIGH); //turn off
  digitalWrite(PIN_G5, HIGH);
  digitalWrite(PIN_F4, HIGH);
  digitalWrite(PIN_E3, HIGH);
  
  //------------------------
  //ZONE2
  
  pinMode(PIN_H1, OUTPUT);
  pinMode(PIN_G0, OUTPUT);
  pinMode(PIN_F1, OUTPUT);
  pinMode(PIN_E2, OUTPUT);
  digitalWrite(PIN_H1, HIGH); //turn
  digitalWrite(PIN_G0, HIGH);
  digitalWrite(PIN_F1, HIGH);
  digitalWrite(PIN_E2, HIGH);

//------------------------
  //ZONE3

  pinMode(PIN_A_1, OUTPUT);
  pinMode(PIN_B0, OUTPUT);
  pinMode(PIN_C1, OUTPUT);
  pinMode(PIN_D2, OUTPUT);
  digitalWrite(PIN_A_1, HIGH); //turn off
  digitalWrite(PIN_B0, HIGH);
  digitalWrite(PIN_C1, HIGH);
  digitalWrite(PIN_D2, HIGH);

//------------------------
  //ZONE4

  pinMode(PIN_A_4, OUTPUT);
  pinMode(PIN_B5, OUTPUT);
  pinMode(PIN_C4, OUTPUT);
  pinMode(PIN_D3, OUTPUT);
  digitalWrite(PIN_A_4, HIGH); //turn off
  digitalWrite(PIN_B5, HIGH);
  digitalWrite(PIN_C4, HIGH);
  digitalWrite(PIN_D3, HIGH);

  
}
//--------------------------------------------------------
bool toggle = true;

void loop() {

  toggle = !toggle;

  //------------------------
  //ZONE1
  
//  digitalWrite(PIN_H4, toggle);
//  digitalWrite(PIN_G5, toggle);
//  digitalWrite(PIN_F4, toggle);
  digitalWrite(PIN_E3, toggle);


  //------------------------
  //ZONE2

//  digitalWrite(PIN_H1, toggle);
//  digitalWrite(PIN_G0, toggle);
//  digitalWrite(PIN_F1, toggle);
  //digitalWrite(PIN_E2, toggle);

//------------------------
  //ZONE3

//  digitalWrite(PIN_A_1, toggle);
//  digitalWrite(PIN_B0, toggle);
//  digitalWrite(PIN_C1, toggle);
//  digitalWrite(PIN_D2, toggle);

//------------------------
  //ZONE4

//  digitalWrite(PIN_A_4, toggle);
//  digitalWrite(PIN_B5, toggle);
//  digitalWrite(PIN_C4, toggle);
  //digitalWrite(PIN_D3, toggle);


  
  delay(15000);

}
