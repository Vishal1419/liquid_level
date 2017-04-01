# liquid_level
automation of water level in a tank using sensors as well as timing

Here is the circuit diagram:

<img src="https://github.com/Vishal1419/liquid_level/blob/master/circuit-diagram.png?raw=true"> </img>

Switches SW1 and SW2 shown in the above diagram are actually sensors as shown in below image:

<img src="https://github.com/Vishal1419/liquid_level/blob/master/Water%20Level%20Liquid%20Sensor%20Float%20Switch%20normally%20OPEN%20type.jpg?raw=true"> </img>

And at last here is the arduino code:

    #include <LiquidCrystal.h>

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
