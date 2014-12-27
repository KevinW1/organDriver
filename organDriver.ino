#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

byte chestChannel = 0x01;  // channel
#define clockPin 12  //pin 13 of chip
#define latchPin 11  //pin 12 of chip
#define dataPin 10  //pin 3 of chip
#define numChips 2  // number of shift registers
#define noteOffset 24  //C1 value, start of rank in midi
byte noteStates[numChips];

//653-A6R-162RF  dip switch model

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

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




