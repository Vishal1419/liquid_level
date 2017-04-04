/*
  01 - GND -------- GND
  02 - +5V -------- 5V
  03 - V0  -------- Potentiometer Middle
  04 - Reset ------ 9
  05 - Read/Write - GND
  06 - Enable ----- 8
  07 - Data0 ------ NC
  08 - Data1 ------ NC
  09 - Data2 ------ NC
  10 - Data3 ------ NC
  11 - Data4 ------ 10
  12 - Data5 ------ 11
  13 - Data6 ------ 12
  14 - Data7 ------ 13
  15 - +LCD ------- 5V
  16 - -LCD ------- GND
*/

//include LCD library
#include <LiquidCrystal.h>
#include "pitches.h"

//Initialize an LCD object
/*Pins should be mentioned in this order:
  Reset
  Enable
  Data4
  Data5
  Data6
  Data7
*/
LiquidCrystal lcd(3, 2, 4, 5, 6, 7);

int offTimePin = A0;
int offTime = 0;
int upSensor = 11;
int downSensor = 10;
int continuousSwitch = 8;
int suddenSwitch = 9;
int siren = 12;

unsigned long counter = 0;
unsigned long motorStartTime = 0;
unsigned long offTimeInMillis = 0;

String lastValue = "OFF";
bool motorInterrupt = false;

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup()
{
  pinMode(offTimePin, INPUT);
  pinMode(upSensor, INPUT);
  pinMode(downSensor, INPUT);
  pinMode(continuousSwitch, OUTPUT);
  pinMode(suddenSwitch, OUTPUT);
  pinMode(siren, OUTPUT);

  digitalWrite(suddenSwitch, LOW);
  digitalWrite(continuousSwitch, LOW);

  /*  Serial.begin(9600); */
  
  //Begin the LCD interface
  lcd.begin(16, 2);

  lcd.print("MOTOR OFF");
  lcd.setCursor(0, 2);
  lcd.print("AUTO OFF: ");
}

void loop()
{
  offTime = map(analogRead(offTimePin), 0, 1023, 1, 120);
  offTimeInMillis = ((long)offTime * 60 * 1000);
  lcd.setCursor(10, 1);
  lcd.print(String(offTime) + " Min");

  unsigned long now = millis();
 
  /*
  Serial.println("now: " + String(now));
  Serial.println("start time: " + String(digitalRead(motorStartTime)));
  Serial.println("off time: " + String(offTime));
  Serial.println("off time in millis: " + String(offTimeInMillis));
  Serial.println("status: " + String(lastValue));
  */

  if(digitalRead(upSensor) == LOW && digitalRead(downSensor) == LOW)
  {
    digitalWrite(continuousSwitch, LOW);
    if(lastValue == "ON")
    {
      lcd.setCursor(6, 0);
      lcd.print("   ");
      lcd.setCursor(6, 0);
      lcd.print("OFF");
      lastValue = "OFF";
    }
    counter = 0;
    motorStartTime = now;
  }
  else if((digitalRead(upSensor) == HIGH && digitalRead(downSensor) == HIGH) && motorInterrupt == false)
  {    
    digitalWrite(continuousSwitch, HIGH);
    if(lastValue == "OFF")
    {
      lcd.setCursor(6, 0);
      lcd.print("   ");
      lcd.setCursor(6, 0);
      lcd.print("ON");
      lastValue = "ON";
    }
    counter++;
  }

  if(counter == 1)
  {
    motorStartTime = now;
    digitalWrite(suddenSwitch, HIGH);
    delay(1000); //1 Second
    digitalWrite(suddenSwitch, LOW);
  }

  if((now - motorStartTime) > (offTimeInMillis))
  {
    digitalWrite(continuousSwitch, LOW);
    if(lastValue == "ON")
    {
      lcd.setCursor(6, 0);
      lcd.print("   ");
      lcd.setCursor(6, 0);
      lcd.print("OFF");
      lastValue = "OFF";
    }
    counter = 0;

    if(digitalRead(downSensor) == HIGH) //No water
    {
      motorInterrupt = true;
      int x = 0;
      while(x < 2)
      {
        // iterate over the notes of the melody:
        for (int thisNote = 0; thisNote < 8; thisNote++)
        {
          // to calculate the note duration, take one second
          // divided by the note type.
          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int noteDuration = 1000 / noteDurations[thisNote];
          tone(siren, melody[thisNote], noteDuration);
      
          // to distinguish the notes, set a minimum time between them.
          // the note's duration + 30% seems to work well:
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
          // stop the tone playing:
          noTone(siren);
        }
      }
    }
  }

}
