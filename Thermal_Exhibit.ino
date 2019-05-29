/*
 * Project: Thermal Properties of Materials Exhibit
 * Description: To display how different materials react under heating. Turns on a heating bulb 
 *              with a push button and heats materials until 40 degrees celsius or until on 30
 *              seconds has passed. Thermistors are used to detect temperature and seven segment
 *              displays are used to display the temperature.
 * Created: May 29 2019
 */


//Below are for TM1637
#include <Arduino.h>
#include <TM1637Display.h>
#include <math.h>

#define CLK 2

#define DIO1 3
#define DIO2 5
#define DIO3 7


#define TEST_DELAY   500
#define MAX_TEMP   30
#define HEAT_INTERVAL   20000UL

TM1637Display display1(CLK, DIO1);
TM1637Display display2(CLK, DIO2);
TM1637Display display3(CLK, DIO3);



//Below are for Thermistor
int ThermistorPin1 = A0, ThermistorPin2 = A1, ThermistorPin3 = A2;
const int buttonPin = 11;
const int relayPin = 12;
const float R1 = 10000;
const float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
const int sevenSegmentBrightness = 0x0f;

unsigned long previousTime;

void setup() {
  Serial.begin(9600);
  uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
  display1.setSegments(data);
  display2.setSegments(data);
  display3.setSegments(data);
  display1.setBrightness(sevenSegmentBrightness);
  display2.setBrightness(sevenSegmentBrightness);
  display3.setBrightness(sevenSegmentBrightness);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // relay off
  pinMode(buttonPin, INPUT_PULLUP);
  previousTime = millis();
  
}

void loop() {
  int T1 = getTemperatureInC(ThermistorPin1);
  int T2 = getTemperatureInC(ThermistorPin2);
  int T3 = getTemperatureInC(ThermistorPin3);
  display1.showNumberDec(T1, false, 2,2);
  display2.showNumberDec(T2, false, 2,2);
  display3.showNumberDec(T3, false, 2,2);
  
  if (T1 >= MAX_TEMP || T2 >= MAX_TEMP || T3 >=MAX_TEMP || millis() - previousTime > HEAT_INTERVAL){
    //Heating bulb will turn off if any material exceeds MAX_TEMP or On-time of bulb has exceeded HEAT_INTERVAL.
    previousTime = millis();
    digitalWrite(relayPin, HIGH); // turn off
  } else if (digitalRead(buttonPin) == HIGH) {
    previousTime = millis();
    digitalWrite(relayPin, LOW); // turn on
  }
  delay(TEST_DELAY);
}

//getTermperature function takes inputed voltage value and outputs temperature value in celsuis
int getTemperatureInC(int pin) {
  int V2 = analogRead(pin);
  //Equations from: http://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
  float R2 = R1 * (1023.0 / (float)V2 - 1.0);  
  float logR2 = log(R2);
  float T3 = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T3 = T3 - 273.15;
  return floor(T3);
}
