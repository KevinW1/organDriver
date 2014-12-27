#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

byte chestChannel = 0x01;
#define clockPin 12  //pin 13 of chip
#define latchPin 11  //pin 12 of chip
#define dataPin 10  //pin 3 of chip
#define numChips 1
byte noteStates[numChips];

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
  for (int j = 0; j < 8; j++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, noteStates[0]);  
    digitalWrite(latchPin, HIGH);
  }
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  int pinNum = pitch % 8;
  bitWrite(noteStates[0], pinNum, 1);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  int pinNum = pitch % 8;
  bitWrite(noteStates[0], pinNum, 0);
}



