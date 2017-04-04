# liquid_level
automation of water level in a tank using sensors as well as timing

<h3>Circuit diagram:</h3>

<img src="https://github.com/Vishal1419/liquid_level/blob/master/schematic.png"> </img>

Switches SW1 and SW2 shown in the above diagram are actually sensors as shown in below image:

<img src="https://github.com/Vishal1419/liquid_level/blob/master/Water%20Level%20Liquid%20Sensor%20Float%20Switch%20normally%20OPEN%20type.jpg?raw=true"> </img>

<h3> Arduino Code: </h3>

    #include <LiquidCrystal.h>
    #include "pitches.h"

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

    int melody[] = {
      NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
    };

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
            for (int thisNote = 0; thisNote < 8; thisNote++)
            {
              int noteDuration = 1000 / noteDurations[thisNote];
              tone(siren, melody[thisNote], noteDuration);

              int pauseBetweenNotes = noteDuration * 1.30;
              delay(pauseBetweenNotes);

              noTone(siren);
            }
          }
        }
      }

    }

<h3> pitches.h </h3>

    #define NOTE_B0  31
    #define NOTE_C1  33
    #define NOTE_CS1 35
    #define NOTE_D1  37
    #define NOTE_DS1 39
    #define NOTE_E1  41
    #define NOTE_F1  44
    #define NOTE_FS1 46
    #define NOTE_G1  49
    #define NOTE_GS1 52
    #define NOTE_A1  55
    #define NOTE_AS1 58
    #define NOTE_B1  62
    #define NOTE_C2  65
    #define NOTE_CS2 69
    #define NOTE_D2  73
    #define NOTE_DS2 78
    #define NOTE_E2  82
    #define NOTE_F2  87
    #define NOTE_FS2 93
    #define NOTE_G2  98
    #define NOTE_GS2 104
    #define NOTE_A2  110
    #define NOTE_AS2 117
    #define NOTE_B2  123
    #define NOTE_C3  131
    #define NOTE_CS3 139
    #define NOTE_D3  147
    #define NOTE_DS3 156
    #define NOTE_E3  165
    #define NOTE_F3  175
    #define NOTE_FS3 185
    #define NOTE_G3  196
    #define NOTE_GS3 208
    #define NOTE_A3  220
    #define NOTE_AS3 233
    #define NOTE_B3  247
    #define NOTE_C4  262
    #define NOTE_CS4 277
    #define NOTE_D4  294
    #define NOTE_DS4 311
    #define NOTE_E4  330
    #define NOTE_F4  349
    #define NOTE_FS4 370
    #define NOTE_G4  392
    #define NOTE_GS4 415
    #define NOTE_A4  440
    #define NOTE_AS4 466
    #define NOTE_B4  494
    #define NOTE_C5  523
    #define NOTE_CS5 554
    #define NOTE_D5  587
    #define NOTE_DS5 622
    #define NOTE_E5  659
    #define NOTE_F5  698
    #define NOTE_FS5 740
    #define NOTE_G5  784
    #define NOTE_GS5 831
    #define NOTE_A5  880
    #define NOTE_AS5 932
    #define NOTE_B5  988
    #define NOTE_C6  1047
    #define NOTE_CS6 1109
    #define NOTE_D6  1175
    #define NOTE_DS6 1245
    #define NOTE_E6  1319
    #define NOTE_F6  1397
    #define NOTE_FS6 1480
    #define NOTE_G6  1568
    #define NOTE_GS6 1661
    #define NOTE_A6  1760
    #define NOTE_AS6 1865
    #define NOTE_B6  1976
    #define NOTE_C7  2093
    #define NOTE_CS7 2217
    #define NOTE_D7  2349
    #define NOTE_DS7 2489
    #define NOTE_E7  2637
    #define NOTE_F7  2794
    #define NOTE_FS7 2960
    #define NOTE_G7  3136
    #define NOTE_GS7 3322
    #define NOTE_A7  3520
    #define NOTE_AS7 3729
    #define NOTE_B7  3951
    #define NOTE_C8  4186
    #define NOTE_CS8 4435
    #define NOTE_D8  4699
    #define NOTE_DS8 4978

<h3> Note: </h3>

Put pitches.h in the same folder as code.ino

<h3> Connections: </h3>

1. P - Phase of Mains supply
2. N - Neutral of Mains supply
3. RLY1/CO - P2 of Motor Starter
4. RLY1/NO - P3 of Motor Starter 
5. RLY2/CO - one end of big green switch of Motor starter
6. RLY2/NO - another end of big green switch of Motor starter
7. D10 - one end of Down sensor
8. D11 - one end of Up sensor
9. +5V - another ends of Up and Down sensors

<b>Assumptions</b>

1. Difference between P and N is 230V
2. RLY1 is continuousRelay
3. RLY2 is suddenRelay

<h3>Tasks completed:</h3>

When Sensors SW1 and SW2 both have their rings down, the motor starts. When both of these sensors have their ringss up, motor stops.
Also, if the auto-off time is reached then motor should stop automatically ignoring the values of sensors.
If the motor is turned off ignoring the sensor values, then motor will never start automatically until SW3 is turned off and turned on again.
When there is no water in tank and motor stops after set time is elapsed, the buzzer should play a melody, so that user is notified that there is no water in the tank, which means motor does not pull any water from bore and so no water in the tank.
