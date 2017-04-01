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

unsigned long counter = 0;
unsigned long motorStartTime = 0;
unsigned long offTimeInMillis = 0;

String lastValue = "OFF";

void setup()
{
  pinMode(offTimePin, INPUT);
  pinMode(upSensor, INPUT);
  pinMode(downSensor, INPUT);
  pinMode(continuousSwitch, OUTPUT);
  pinMode(suddenSwitch, OUTPUT);

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
  
  if(counter == 1)
  {
    motorStartTime = now;
    digitalWrite(suddenSwitch, HIGH);
    delay(1000); //1 Second
    digitalWrite(suddenSwitch, LOW);
  }

  /*
  Serial.println("now: " + String(now));
  Serial.println("start time: " + String(digitalRead(motorStartTime)));
  Serial.println("off time: " + String(offTime));
  Serial.println("off time in millis: " + String(offTimeInMillis));
  Serial.println("status: " + String(lastValue));
  */

  if((digitalRead(upSensor) == LOW && digitalRead(downSensor) == LOW) || (now - motorStartTime) > (offTimeInMillis))
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
  }
  else if(digitalRead(upSensor) == HIGH && digitalRead(downSensor) == HIGH)
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
}
