//Ultrasonic Sensor Pins
const int trigPin = 5;
const int echoPin = 6;
//Yellow LED Pins - Streetlights
const int LampLED1 = 7;
const int LampLED2 = 8;
const int LampLED3 = 9;
//Red LED Pins - Speed Warning
const int SpeedLED1 = 10;
const int SpeedLED2 = 11;
const int SpeedLED3 = 12;

//Declaring Required Variables
float Speed = 0, Distance = 0, distance1 = 0, distance2 = 0, Time = 0, time1 = 0, time2 = 0;
int flag1 = 0, flag2 = 0, countLEDOn = 0, countLEDOff = 0, LEDStates = 0;

const unsigned long interval = 500;  //Blink Interval In Milliseconds
unsigned long previousMillis = 0;    //Stores The Last Time Red LEDs Were Toggled

void setup() {
  //Setting Up Input And Output Pins
  Serial.begin(9600);
  pinMode(LampLED1, OUTPUT);
  pinMode(LampLED2, OUTPUT);
  pinMode(LampLED3, OUTPUT);
  pinMode(SpeedLED1, OUTPUT);
  pinMode(SpeedLED2, OUTPUT);
  pinMode(SpeedLED3, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  //Detecting Object Using Ultrasonic Sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //Measuring Distance Of Object
  unsigned long time = pulseIn(echoPin, HIGH);
  Distance = time / 28.5 / 2;

  //Taking Real Time Using millis() Function
  unsigned long realTime = millis();
  realTime = realTime / 1000.00;  //Converting Millisecond To Second

  //Vehicle At Point A
  if (Distance <= 300 && Distance > 295 && flag1 == 0 && flag2 == 0) {
    time1 = millis();
    distance1 = Distance;
    flag1 = 1;

    //Serial.print("Time 1: ");
    //Serial.println(time1);
    //Serial.print("Distance 1: ");
    //Serial.println(distance1);
  }

  //Vehicle At Point B
  if (Distance <= 100 && Distance > 95 && flag1 == 1 && flag2 == 0) {
    time2 = millis();
    distance2 = Distance;
    flag2 = 1;

    //Serial.print("Time 2: ");
    //Serial.println(time2);
    //Serial.print("Distance 2: ");
    //Serial.println(distance2);
  }

  //Speed Calculation
  if (flag1 == 1 && flag2 == 1) {
    Time = ((time2 - time1) / 1000);           //Converting Millisecond To Second
    Speed = ((distance1 - distance2) / Time);  //Speed = Distance / Time

    //Serial.print("Speed in Cm/s: ");
    //Serial.println(Speed);
    //Serial.print("Real Time: ");
    //Serial.println(realTime);
    //Serial.println(" ");

    flag1 = 2, flag2 = 2;
  }

  //Speed Waring Conditions
  unsigned long currentMillis = millis();

  //Speed Limit Set To 180 Cm/s
  if (Speed > 180 && Time != 0) {
    //Blink Red LEDs Based On Interval
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (LEDStates == 0) {
        LEDStates = 1;
        digitalWrite(SpeedLED1, HIGH);
        digitalWrite(SpeedLED2, HIGH);
        digitalWrite(SpeedLED3, HIGH);
        countLEDOn++;
      } else {
        LEDStates = 0;
        digitalWrite(SpeedLED1, LOW);
        digitalWrite(SpeedLED2, LOW);
        digitalWrite(SpeedLED3, LOW);
        countLEDOff++;
      }
    }
  } else if (Speed < 180 && Speed != 0) {
    //Reset Values
    Speed = 0;
    Distance = 0;
    distance1 = 0;
    distance2 = 0;
    Time = 0;
    time1 = 0;
    time2 = 0;
    flag1 = 0;
    flag2 = 0;
  }

  //Detecting Brightness Using LDR Sensor
  int brightness = analogRead(A0);
  //Serial.print("Brightness: ");
  //Serial.print(brightness);

  //Streetlight Conditions
  //Brightness Set To 800 And Object Detection At 300 Cm
  if (brightness < 800 && Distance <= 300) {
    digitalWrite(LampLED1, HIGH);
    digitalWrite(LampLED2, HIGH);
    digitalWrite(LampLED3, HIGH);
  } else {
    digitalWrite(LampLED1, LOW);
    digitalWrite(LampLED2, LOW);
    digitalWrite(LampLED3, LOW);
  }

  //Reset Values
  if (flag1 == 2 && flag2 == 2 && countLEDOn == 3 && countLEDOff == 3) {
    Speed = 0;
    Distance = 0;
    distance1 = 0;
    distance2 = 0;
    Time = 0;
    time1 = 0;
    time2 = 0;
    flag1 = 0;
    flag2 = 0;
    countLEDOn = 0;
    countLEDOff = 0;
    LEDStates = 0;
  }
}