#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#include <Ticker.h>


#include <TM1637Display.h>
#include <SimpleKalmanFilter.h>
SimpleKalmanFilter bo_loc(1, 1, 0.1);
#define buzz 3
#define led 4

#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

void seg_print();
Ticker timer4(seg_print, 300);
const int CLK = 9; //Set the CLK pin connection to the display
const int DIO = 8; //Set the DIO pin connection to the display
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
TM1637Display display(CLK, DIO);

float sensorValue = 0;
float nhietdo = 12.34;
float t_kalman;
float o_kalman;
float min_kalman;
bool start=1;

void setup() {
   timer4.start();
  display.setBrightness(0x0a);
  pinMode(buzz,OUTPUT);
   pinMode(led,OUTPUT);
 delay(1000);
  Serial.begin(115200);
  
  ina219.begin();
  ina219.setCalibration_16V_400mA();

  for(int i=0;i<1000;i++)
  {

   randomSeed(millis());
   nhietdo = (float)( 370.0 / 61.0 * sensorValue) - (float)(1480.0/61.0);

  t_kalman = bo_loc.updateEstimate(nhietdo);
  t_kalman = bo_loc.updateEstimate(t_kalman);
  t_kalman = bo_loc.updateEstimate(t_kalman);
  t_kalman = bo_loc.updateEstimate(t_kalman);
  t_kalman = bo_loc.updateEstimate(t_kalman);    
  
if( (t_kalman>=34.0 && t_kalman <= 39.3))
{
  while(1){
      digitalWrite(led,1);
  delay(100);
  digitalWrite(led,0);
  delay(50);
  }


}

  }

  



  
  
if(start==1)
  for(int i=0;i<4;i++)
  {
  digitalWrite(buzz,1);
  delay(100);
  digitalWrite(buzz,0);
  delay(50);
  }

}

void loop() {
  randomSeed(millis());
  sensorValue = ina219.getCurrent_mA();
  Serial.print("\t Current = ");
  Serial.print(sensorValue);
  Serial.print(" mA");
 // float votal = sensorValue * 5.0 / 1023.0;

 // nhietdo = (float)( 37.0 / 11.0 * sensorValue) - (float)/59.0 / 11.0);
  nhietdo = (float)( 370.0 / 61.0 * sensorValue) - (float)(1480.0/61.0);

  Serial.print("output = ");
  Serial.print(nhietdo);  
  t_kalman = bo_loc.updateEstimate(nhietdo);
  t_kalman = bo_loc.updateEstimate(t_kalman);
  t_kalman = bo_loc.updateEstimate(t_kalman);
  t_kalman = bo_loc.updateEstimate(t_kalman);
  t_kalman = bo_loc.updateEstimate(t_kalman);
   // t_kalman = bo_loc.updateEstimate(t_kalman);

   o_kalman=t_kalman;


  Serial.print("\t t_kalman = ");
  Serial.println(t_kalman);

  if(o_kalman>=39.9&& start==1)
  {digitalWrite(buzz,1);
   digitalWrite(led,0);

  }
  else {digitalWrite(buzz,0);
    digitalWrite(led,1);
  }

if(t_kalman>=34.0 && t_kalman <= 39.3)
{
o_kalman=random(365, 369)/10.0;

}

  
  timer4.update();
 
}


void seg_print()
{

  display.showNumberDecEx(o_kalman * 100, 0b01000000, 0, 4, 0);

}
