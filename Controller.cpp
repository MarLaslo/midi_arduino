#include "Arduino.h"

#include "Controller.h"


Button::Button(byte pin, byte debounce, byte type = 0){
  _pin = pin;
  pinMode(_pin, INPUT_PULLUP);
  _debounce = debounce;
  _type = type;
  _time = 0;
  _busy = false;
  _status = 0b00000010;
  _last = 1;
}

byte Button::getValue(){
  // If BUSY bit not set - read button
  if (bitRead(_status, 0) == false) { // If busy false
    if (digitalRead(_pin) == _last) return 2; // If same as last state - exit
  }

  // If NEW Bit set - Key just pressed, record time
  if (bitRead(_status, 1) == true) { // If new is true
    bitSet(_status, 0); // Set busy TRUE
    bitClear(_status, 1); // Set New FALSE
    _time = millis();
    return 255;
  }

  // Check if debounce time has passed - If no, exit
  if (millis() - _time < _debounce) return 255;

  // Debounce time has passed. Read pin to see if still set the same
  // If it has changed back - assume false alarm
  if (digitalRead(_pin) == _last) {
    bitClear(_status, 0); // Set busy false
    bitSet(_status, 1); // Set new true
    return 255;
  }

  // If this point is reached, event is valid. return event type
  else {
    bitClear(_status, 0); // Set busy false
    bitSet(_status, 1); // Set new true
    _last = ((~_last) & 0b00000001); // invert _last
    return _last;
  }
}

byte Button::getType(){
  return _type;
}
//********************************************************************
Pot::Pot(byte pin, byte command, byte control, byte channel){
  _pin = pin;
  _control = control;
  _value = analogRead(_pin);
  _value = _value >> 3;
  _oldValue = _value << 3;
  _value = _value << 3;
  Pcommand = command;
  Pcontrol = control;
  Pchannel = channel;
}

byte Pot::getValue(){
  _value = analogRead(_pin);
  int tmp = (_oldValue - _value);
  if (tmp >= 8 || tmp <= -8) {
    _oldValue = _value >> 3;
    _oldValue = _oldValue << 3;
    return _value >> 3;
  }
  return 255;
}

void Pot::setChannel(byte channel) {
  Pchannel = channel;
}
//**********************************************************************
Mux::Mux(byte outpin_, byte numPins_, bool analog_)
{
  outpin = outpin_;
  numPins = numPins_;
  analog = analog_;
  if (analog == false) {
    pinMode(outpin, INPUT_PULLUP);
    }
  else {
    pinMode(outpin, INPUT);
    }

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  if (numPins > 8) pinMode(9, OUTPUT);
}
