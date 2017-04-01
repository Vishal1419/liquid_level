# liquid_level
automation of water level in a tank using sensors as well as timing

<h3>Circuit diagram:</h3>

<img src="https://github.com/Vishal1419/liquid_level/blob/master/circuit-diagram.png?raw=true"> </img>

Switches SW1 and SW2 shown in the above diagram are actually sensors as shown in below image:

<img src="https://github.com/Vishal1419/liquid_level/blob/master/Water%20Level%20Liquid%20Sensor%20Float%20Switch%20normally%20OPEN%20type.jpg?raw=true"> </img>

<h3> Arduino Code: </h3>

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
