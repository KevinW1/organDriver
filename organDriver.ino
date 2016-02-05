#include <MIDI.h>
#include <string.h>
MIDI_CREATE_DEFAULT_INSTANCE();
//that old 74HC595N shift register
#define latchPin 11 //pin 12 of chip
#define clockPin 10 //pin 11 of chip
#define dataPin 12 //pin 14 of chip
#define outputEnable 13 //pin 13, inverted
#define numChips 8  // number of shift registers
#define noteOffset 36 //midi C1 value, start of rank
#define noteLEDPin 6 //LED for blinking note light

#define dipPin1 2 //|pins for dip switch model 653-A6R-162RF
#define dipPin2 5 //|
#define dipPin4 4 //|
#define dipPin8 3 //|

byte chestChannel = 0x00; // channel, 0 is omni
byte noteStates[numChips]; //note status array
uint8_t activeNote = 0; //for checking to see if any notes are on


void setup()
{
  pinMode(outputEnable, OUTPUT);
  digitalWrite(outputEnable, HIGH); //turn off outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(noteLEDPin, OUTPUT);

  pinMode(dipPin1, INPUT_PULLUP);
  pinMode(dipPin2, INPUT_PULLUP);
  pinMode(dipPin4, INPUT_PULLUP);
  pinMode(dipPin8, INPUT_PULLUP);

  setChannel();

  memset(noteStates, 0, sizeof(noteStates)); // clear the note array
  shiftShit(); //sets everything to zero
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(chestChannel);
  digitalWrite(noteLEDPin, LOW); //turn off note light
  digitalWrite(outputEnable, LOW); //turn on outputs
}


void setChannel() {
  bitWrite(chestChannel, 0, !digitalRead(dipPin1));
  bitWrite(chestChannel, 1, !digitalRead(dipPin2));
  bitWrite(chestChannel, 2, !digitalRead(dipPin4));
  bitWrite(chestChannel, 3, !digitalRead(dipPin8));
  chestChannel += 1;  //because this library wants channel numbers 1-16, not 0-15
}

void loop()
{
  MIDI.read();

  shiftShit();
}
void shiftShit()
{
  activeNote = 0;
  digitalWrite(latchPin, LOW);
  for (int i = 0; i < numChips; i++)
  {
    shiftOut(dataPin, clockPin, MSBFIRST, noteStates[i]);
    activeNote += noteStates[i];
  }
  digitalWrite(latchPin, HIGH);
  if (activeNote == 0) {
    digitalWrite(noteLEDPin, LOW);
  }
  else {
    digitalWrite(noteLEDPin, HIGH);
  }
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
  int chipNum = (pitch / 8);
  int pinNum = pitch % 8;
  bitWrite(noteStates[chipNum], pinNum, state);
}


