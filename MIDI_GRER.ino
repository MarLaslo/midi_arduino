#include <USB-MIDI.h>
#include "Controller.h"

USBMIDI_CREATE_DEFAULT_INSTANCE();

byte NUMBER_BUTTONS = 2;
byte NUMBER_POTS = 2;

byte channelCount = 1;

//***DEFINE DIRECTLY CONNECTED POTENTIOMETERS************************
//Pot (Pin Number, Command, CC Control, Channel Number)

Pot PO1(A0, 0, 3, 1);
Pot PO2(A1, 0, 9, 1);
//Add pots used to array below like this->  Pot *POTS[] {&PO1, &PO2, &PO3, &PO4, &PO5, &PO6};
Pot *POTS[] {&PO1, &PO2};
//*******************************************************************


//***DEFINE DIRECTLY CONNECTED BUTTONS*******************************
//Button (Pin Number, Debounce Time, Type)
//Types: 0 - NoType, 1 - Next Channel, 2 - Previous Channel

Button BU1(2,5,1);
Button BU2(3,5,2);
//*******************************************************************
//Add buttons used to array below like this->  Button *BUTTONS[] {&BU1, &BU2, &BU3, &BU4, &BU5, &BU6, &BU7, &BU8};
Button *BUTTONS[] {&BU1, &BU2};
//*******************************************************************



void setup() {
  MIDI.begin(MIDI_CHANNEL_OFF);
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  if (NUMBER_BUTTONS != 0) updateButtons();
  if (NUMBER_POTS != 0) updatePots();
}

//*****************************************************************

void nextChannel() {
  channelCount++;
  if (channelCount <= 128){
    for (int i =0; i< NUMBER_POTS; i++){
      POTS[i]->setChannel(channelCount);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println(channelCount);
    }
  } 
  else{
    channelCount = 1;
    for (int i =0; i< NUMBER_POTS; i++){
      POTS[i]->setChannel(channelCount);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println(channelCount);
    }
  }
}

void previousChannel() {
  channelCount--;
  if (channelCount >= 1 && channelCount <= 128){
    for (int i =0; i< NUMBER_POTS; i++){
      POTS[i]->setChannel(channelCount);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println(channelCount);
    }
  } 
  else{
    channelCount = 128;
    for (int i =0; i< NUMBER_POTS; i++){
      POTS[i]->setChannel(channelCount);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println(channelCount);
    }
  }
}

//*****************************************************************
void updateButtons() {

  // Cycle through Button array
  for (int i = 0; i < NUMBER_BUTTONS; i = i + 1) {
    byte message = BUTTONS[i]->getValue();

    //  Button is pressed
    if (message == 0) {
      if (BUTTONS[i]->getType() == 1)
          nextChannel();


      if (BUTTONS[i]->getType() == 2)
          previousChannel();
      }
    
    //  Button is not pressed
    if (message == 1) {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

//***********************************************************************
void updatePots() {
  for (int i = 0; i < NUMBER_POTS; i = i + 1) {
    byte potmessage = POTS[i]->getValue();
    if (potmessage != 255) MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel);
  }
}

