
//Below are for TM1637
#include <Arduino.h>
#include <TM1637Display.h>
#include <math.h>

#define CLK1 2
#define CLK2 4
#define CLK3 6



#define DIO1 3
#define DIO2 5
#define DIO3 7



// The amount of time (in milliseconds) between tests
#define TEST_DELAY   500
#define MAX_TEMP   25

TM1637Display display1(CLK1, DIO1);
TM1637Display display2(CLK2, DIO2);
TM1637Display display3(CLK3, DIO3);



//Below are for Thermistor
int ThermistorPin1 = A0 , ThermistorPin2 = A1, ThermistorPin3 = A2;
int Vo, V1, V2, T_new;
float R1 = 10000;
float logR2, R2, T, T1;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;




void setup() {
Serial.begin(9600);
uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
display1.setSegments(data);
display2.setSegments(data);
display3.setSegments(data);
pinMode(13, OUTPUT);
digitalWrite(13,HIGH);
}

void loop() {

  display1.setBrightness(0x0f);
  display2.setBrightness(0x0f);
  display3.setBrightness(0x0f);

  //Thermistor 1
  Vo = analogRead(ThermistorPin1);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T1 = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T1 = T1 - 273.15;
  int T_Rounded1 = round(T1);

  Serial.println(T_Rounded1);
  display1.showNumberDec(T_Rounded1, false, 2,2);


  //Thermistor 2
  V1 = analogRead(ThermistorPin2);
  R2 = R1 * (1023.0 / (float)V1 - 1.0);
  logR2 = log(R2);
  int T2 = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T2 = T2 - 273.15;
  int T_Rounded2 = round(T2);

  display2.showNumberDec(T_Rounded2, false, 2,2);
  delay(TEST_DELAY);


  //Thermistor 3
  V2 = analogRead(ThermistorPin3);
  R2 = R1 * (1023.0 / (float)V2 - 1.0);
  logR2 = log(R2);
  int T3 = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T3 = T3 - 273.15;
  int T_Rounded3 = round(T3);

  display3.showNumberDec(T_Rounded3, false, 2,2);

  if (T1 > MAX_TEMP || T2 > MAX_TEMP || T3 > MAX_TEMP){
    digitalWrite(13, LOW);

  }
  else {
    digitalWrite(13, HIGH);
  }
  delay(TEST_DELAY);
 
}
