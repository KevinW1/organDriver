#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

byte chestChannel = 0x00;  // channel
#define clockPin 12  //pin 13 of chip
#define latchPin 11  //pin 12 of chip
#define dataPin 10  //pin 3 of chip
#define numChips 2  // number of shift registers
#define noteOffset 24  //C1 value, start of rank in midi
#define dipPin1 9
#define dipPin2 8
#define dipPin4 7
#define dipPin8 6

byte noteStates[numChips];

//653-A6R-162RF  dip switch model

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(dipPin1, INPUT_PULLUP);
  pinMode(dipPin2, INPUT_PULLUP);
  pinMode(dipPin4, INPUT_PULLUP);
  pinMode(dipPin8, INPUT_PULLUP);

  setChannel();

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(chestChannel);

}

void loop() {

  MIDI.read();
  shiftShit();

} 

void shiftShit(){
  digitalWrite(latchPin, LOW);
  for (int i = 0; i < numChips; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, noteStates[i]);  
  }
  digitalWrite(latchPin, HIGH);
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  updateNoteStatus(pitch, true);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  updateNoteStatus(pitch, false);
}

void updateNoteStatus(byte pitch, boolean state)
{
  pitch -= noteOffset;
  pitch = (pitch < 0) ? 0 : pitch;
  int chipNum = (pitch/8);
  int pinNum = pitch % 8;
  bitWrite(noteStates[chipNum], pinNum, state);

}

void setChannel()
{
  bitWrite(chestChannel, 0, !digitalRead(dipPin1));
  bitWrite(chestChannel, 1, !digitalRead(dipPin2));
  bitWrite(chestChannel, 2, !digitalRead(dipPin4));
  bitWrite(chestChannel, 3, !digitalRead(dipPin8));
}





