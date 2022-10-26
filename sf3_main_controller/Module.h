
//represents a frame module, from main controller's perspective

class Module {
  
  public:
    Module(int pin);
    void turnOn();
    void turnOff();
    void update();
    String getId();
    bool isOn();
    
  private:
    int  _pin;
    bool  _isOn;
    long _timeOn; //record on time
    long _timeOff; //record off time
    bool _initialRun;
};

// constructor
Module::Module(int pin) {
  this->_pin = pin;
  this->_isOn = false;
  this->_initialRun = true;
  this->_timeOn  = 0; //record on time
  this->_timeOff = 0; //record off time
}

void Module::update() { //ensure this is ALWAYS called in main loop

  //auto turns OFF after MAX_TIME_ON
  
  if(millis() - this->_timeOn > MAX_TIME_ON) {
    this->turnOff();
  }
}

void Module::turnOn() {
  if(millis() - this->_timeOff > MIN_TIME_OFF || this->_initialRun) {
    this->_initialRun = false;
    this->_isOn = true;
    this->_timeOn = millis();
    digitalWrite(this->_pin, LOW);
  } else {
    Serial.println("Cannot turn ON module. Not OFF for enough time.");
  }
}

void Module::turnOff() {
  this->_isOn = false;
  this->_timeOff = millis();
  digitalWrite(this->_pin, HIGH);
}

bool Module::isOn() {
  return this->_isOn;
}
