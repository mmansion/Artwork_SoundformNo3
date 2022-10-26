class LedLight {
  
  public:
    LedLight(int pwm_pin);

    void update();
    int  getBrightness();
    void setBrightness(int brightness);
    void setTargetBrightness(int targetBrightness);
    void setLightningMode(bool onOff);
    
  private:
  
    int _brightness;
    int _pwm_pin;

    int _targetBrightness;
    bool _lightningMode = false;
};

LedLight::LedLight(int pwm_pin) {
  this->_pwm_pin = pwm_pin;     //set pin
  this->_brightness = 0;        // init brightness to 0
  this->_targetBrightness = 0;  //init target brightness
};

void LedLight::update() {

  if(this->_lightningMode) {

    if(this->_brightness == 0) {
      int glitch = random(RANDOM_LIGHTNING_ODDS); //0-ODDS
      if(glitch == 0) {
        this->_brightness = LIGHTNING_BRIGHTNESS;
      }
    } else {
      this->_brightness-= LIGHTNING_FADE_SPEED; //fast fade
      if(this->_brightness < 0) this->_brightness = 0;
    }
    
  } else {
    
    //animate to target brightness
    if(this->_brightness != this->_targetBrightness) {
      
      //fade in
      if(this->_brightness < this->_targetBrightness) {
        this->_brightness += FADE_SPEED; //increase
        if(this->_brightness < 0) {
          this->_brightness = 0;
        }
  
      // fade out
      } else if(this->_brightness > this->_targetBrightness) {
        this->_brightness -= FADE_SPEED; //decrease
        if(this->_brightness > MAX_BRIGHTNESS) {
          this->_brightness = MAX_BRIGHTNESS;
        }
      }
    }
  }

  analogWrite(this->_pwm_pin, this->_brightness);
  
  delay(10); //pin set recovery time
};


int LedLight::getBrightness() {
  return this->_brightness;
};

// sets immediate brightness
void LedLight::setBrightness(int brightness) {
  
  if(brightness > MAX_BRIGHTNESS) { 
    //don't go over max brightness
    this->_brightness = MAX_BRIGHTNESS;
  } else if(brightness < 0) {
    //don't go under min brightness
    this->_brightness = 0;
  } else {
    this->_brightness = brightness;
  }
  
  this->_targetBrightness = brightness;//immediately at target
};

// sets target brightness to fade to
void LedLight::setTargetBrightness(int targetBrightness) {
  
  if(targetBrightness > MAX_BRIGHTNESS) { 
    //don't go over max brightness
    this->_targetBrightness = MAX_BRIGHTNESS;
  } else if(targetBrightness < 0) {
    //don't go under min brightness
    this->_targetBrightness = 0;
  } else {
    this->_targetBrightness = targetBrightness;
  }
};

void LedLight::setLightningMode(bool onOff) {
  this->_lightningMode = onOff;
}
