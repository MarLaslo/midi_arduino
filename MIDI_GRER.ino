#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <USB-MIDI.h>
#include "Controller.h"

USBMIDI_CREATE_DEFAULT_INSTANCE();
LiquidCrystal_I2C lcd(0x27,20,4);


//***NUMBER OF COMPONENTS***//
byte NUMBER_BUTTONS = 2;
byte NUMBER_POTS = 16;

//***CHANNEL COUNTER***
byte channelCount = 1;

//***DEFINE CONNECTED MULTIPLEXER****
//M (Pin Number, Number of inputs, Analog)
Mux M1(A5,16,true);
int Mux1_State[16] = {0};

//***DEFINE CONNECTED POTENTIOMETERS************************
//Pot (Pin Number, Command, CC Control, Channel Number)

Pot PO1(A5, 0, 3, 1);
Pot PO2(A5, 0, 9, 1);
Pot PO3(A5, 0, 21, 1);
Pot PO4(A5, 0, 22, 1);
Pot PO5(A5, 0, 23, 1);
Pot PO6(A5, 0, 24, 1);
Pot PO7(A5, 0, 25, 1);
Pot PO8(A5, 0, 26, 1);
Pot PO9(A5, 0, 27, 1);
Pot PO10(A5, 0, 28, 1);
Pot PO11(A5, 0, 29, 1);
Pot PO12(A5, 0, 30, 1);
Pot PO13(A5, 0, 31, 1);
Pot PO14(A5, 0, 32, 1);
Pot PO15(A5, 0, 33, 1);
Pot PO16(A5, 0, 34, 1);


//Add pots used to array below like this->  Pot *POTS[] {&PO1, &PO2, &PO3, &PO4, &PO5, &PO6};
Pot *POTS[] {&PO1, &PO2, &PO3, &PO4, &PO5, &PO6, &PO7, &PO8, &PO9, &PO10, &PO11, &PO12, &PO13, &PO14, &PO15, &PO16};
//*******************************************************************


//***DEFINE DIRECTLY CONNECTED BUTTONS*******************************
//Button (Pin Number, Debounce Time, Type)
//Types: 0 - NoType, 1 - Next Channel, 2 - Previous Channel

Button BU1(4,5,1);
Button BU2(5,5,2);
//*******************************************************************
//Add buttons used to array below like this->  Button *BUTTONS[] {&BU1, &BU2, &BU3, &BU4, &BU5, &BU6, &BU7, &BU8};
Button *BUTTONS[] {&BU1, &BU2};
//*******************************************************************



void setup() {
  MIDI.begin(MIDI_CHANNEL_OFF);
  lcd.init();
  lcd.backlight();
  printChannel();
  /*
  //***MONITORING
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  */
}

void loop() {
  if (NUMBER_BUTTONS != 0) updateButtons();
  //if (NUMBER_POTS != 0) updatePots();
  updateMux();
  //printMuxPotValues();
}

//***CHANNEL CONTROLLER***************************************

void nextChannel() {
  channelCount++;
  if (channelCount <= 128){
    for (int i =0; i< NUMBER_POTS; i++){
      POTS[i]->setChannel(channelCount);
      //digitalWrite(LED_BUILTIN, HIGH);
      //Serial.println(channelCount);
    }
  } 
  else{
    channelCount = 1;
    for (int i =0; i< NUMBER_POTS; i++){
      POTS[i]->setChannel(channelCount);
      //digitalWrite(LED_BUILTIN, HIGH);
      //Serial.println(channelCount);
    }
  }
}

void previousChannel() {
  channelCount--;
  if (channelCount >= 1 && channelCount <= 128){
    for (int i =0; i< NUMBER_POTS; i++){
      POTS[i]->setChannel(channelCount);
      //digitalWrite(LED_BUILTIN, HIGH);
      //Serial.println(channelCount);
    }
  } 
  else{
    channelCount = 128;
    for (int i =0; i< NUMBER_POTS; i++){
      POTS[i]->setChannel(channelCount);
      //digitalWrite(LED_BUILTIN, HIGH);
      //Serial.println(channelCount);
    }
  }
}

//***BUTTONS UPDATE*******************************************
void updateButtons() {

  // Cycle through Button array
  for (int i = 0; i < NUMBER_BUTTONS; i = i + 1) {
    byte message = BUTTONS[i]->getValue();

    //  Button is pressed
    if (message == 0) {
      if (BUTTONS[i]->getType() == 1) {
        nextChannel();
        printChannel();
      }
      if (BUTTONS[i]->getType() == 2) {
      previousChannel();
      printChannel();
      }
      }
    
    //  Button is not pressed
    if (message == 1) {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

//***POT UPDATE***********************************************
void updatePots() {
  for (int i = 0; i < NUMBER_POTS; i = i + 1) {
    byte potmessage = POTS[i]->getValue();
    if (potmessage != 255) MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel);
  }
}

//***MUX UPDATE***********************************************
void updateMux() {
  for (int i = 0; i < 16; i++) {
    digitalWrite(6, (i & B00000001) ? HIGH : LOW);
    digitalWrite(7, (i & B00000010) ? HIGH : LOW);
    digitalWrite(8, (i & B00000100) ? HIGH : LOW);
    digitalWrite(9, (i & B00001000) ? HIGH : LOW);
    byte potmessage = POTS[i]->getValue();
    //Mux1_State[i] = potmessage;
    if (potmessage != 255) MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel);
  }
}

//***DISPLAY TEXT ON LCD**************************************
void printChannel() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Channel:");
  lcd.setCursor(9,0);
  lcd.print(channelCount);
}

//***MONITORING MULTIPLEXER OUTPUT****************************

/*
void printMuxPotValues() {
  for(int i = 0; i < 16; i ++) {
    if(i == 15) {
      Serial.println(Mux1_State[i]);
    } else {
      Serial.print(Mux1_State[i]);
      Serial.print(",");
    }
  }
}
*/