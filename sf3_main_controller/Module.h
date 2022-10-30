
//represents a frame module, from main controller's perspective

class Module {
  
  public:
    Module(int pin, int id);
    void turnOn();
    void turnOff();
    void update();
    String getId();
    bool isOn();
    void leaveOn();
    int id;
    
  private:
    int  _pin;
    bool  _isOn;
    bool _leaveOn;
    long _timeOn; //record on time
    long _timeOff; //record off time
    bool _initialRun;
};

// constructor
Module::Module(int pin, int id) {
  this->id = id;
  this->_pin = pin;
  this->_isOn = false;
  this->_initialRun = true;
  this->_timeOn  = 0; //record on time
  this->_timeOff = 0; //record off time
  this->_leaveOn = false;
}

void Module::update() { //ensure this is ALWAYS called in main loop

  //auto turns OFF after MAX_TIME_ON
  
  if(this->_isOn && millis() - this->_timeOn > MAX_TIME_ON && !this->_leaveOn) {
    this->turnOff();
  }
}

void Module::leaveOn() { //remains on until turnOff is called explicitly 
  Serial.print("leaving on " );
  Serial.println(this->id);
  
  this->_leaveOn = true;
  this->_isOn = true;
  this->_timeOn = millis();
  this->_initialRun = false;
  digitalWrite(this->_pin, LOW);
}

void Module::turnOn() {
  
    Serial.print("turning on " );
    Serial.println(this->id);
  
    this->_initialRun = false;
    this->_isOn = true;
    this->_timeOn = millis();
    digitalWrite(this->_pin, LOW);

}

void Module::turnOff() {
  Serial.print("turning off " );
  Serial.println(this->id);
  this->_leaveOn = false;
  this->_isOn = false;
  this->_timeOff = millis();
  digitalWrite(this->_pin, HIGH);
  delay(10);
}

bool Module::isOn() {
  return this->_isOn;
}
