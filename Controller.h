#ifndef Controller_h
#define Controller_h

#include <Arduino.h>
class Button
{
  public:
    Button(byte pin, byte debounce, byte type = 0);
    byte getValue();
    byte getType();
    void newValue(byte command, byte value, byte channel);

  private:
    byte _type;
    byte _previous;
    byte _current;
    unsigned long _time;
    int _debounce;
    byte _pin;
    byte _value;
    byte _command;
    bool _busy;
    byte _status;
    byte _last;
    byte _enablepin;
};
//*************************************************************************
class Pot
{
  public:
    Pot(byte pin, byte command, byte control, byte channel);
    void muxUpdate();
    void setChannel(byte channel);
    byte getValue();
    byte Pcommand;
    byte Pcontrol;
    byte Pchannel;

  private:
    byte _pin;
    byte _muxpin;
    byte _numMuxPins;
    byte _control;
    int _value;
    int _oldValue;
    bool _changed;
    byte _enablepin;
};
//*************************************************************************
class Mux
{
  public:
    Mux(byte outpin_, byte numPins_, bool analog_);
    byte outpin;
    byte numPins;
    bool analog;
};
#endif
