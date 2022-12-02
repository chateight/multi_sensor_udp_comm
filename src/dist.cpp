/*
  Reading distance from the laser based VL53L1X
  By: Nathan Seidle
  SparkFun Electronics
  Date: April 4th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  SparkFun labored with love to create this code. Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/14667

  This example prints the distance to an object.

  Are you getting weird readings? Be sure the vacuum tape has been removed from the sensor.
*/

//Optional interrupt and shutdown pins.
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3

#include "module.h"
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X
SFEVL53L1X distanceSensor;

//Uncomment the following line to use the optional shutdown and interrupt pins.
//SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);

const int port = 2;   // GPIO PORT
const int dist_th = 200;

int setup_dist()
{
  int ret_dist = 0;
  M5.lcd.setTextSize(2);  // Set the text size to 2. 
  M5.Lcd.setTextColor(WHITE);     // set font color to white

  Serial.println("VL53L1X Qwiic Test");

  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    return ret_dist;
  }
  Serial.println("Sensor online!");
  // initialise the port
  pinMode(port, OUTPUT);  
  digitalWrite(port, LOW);
  ret_dist = 1;
  return ret_dist;
}

void loop_dist()
{
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }
  int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();

  Serial.print("Distance(mm): ");
  Serial.print(distance);
  M5.lcd.fillScreen(BLACK);   // clear screen
  M5.Lcd.setCursor(35, 100);
  M5.Lcd.printf("distance : %4d mm", distance);

  // alarm externally
  if(distance < dist_th){
    digitalWrite(port, HIGH);
    delay(1);
    digitalWrite(port, LOW);
  }

  float distanceInches = distance * 0.0393701;
  float distanceFeet = distanceInches / 12.0;

  Serial.print("\tDistance(ft): ");
  Serial.print(distanceFeet, 2);

  int signalRate = distanceSensor.getSignalRate();
  Serial.print("\tSignal rate: ");
  Serial.print(signalRate);

  byte rangeStatus = distanceSensor.getRangeStatus();
  Serial.print("\tRange Status: ");

  //Make it human readable
  switch (rangeStatus)
  {
  case 0:
    Serial.print("Good");
    break;
  case 1:
    Serial.print("Sigma fail");
    break;
  case 2:
    Serial.print("Signal fail");
    break;
  case 7:
    Serial.print("Wrapped target fail");
    break;
  default:
    Serial.print("Unknown: ");
    Serial.print(rangeStatus);
    break;
  }
  distanceSensor.setROI(16, 16, 199);
  int roi_x = distanceSensor.getROIX();
  int roi_y = distanceSensor.getROIY();
  Serial.print("\tROI: ");
  Serial.print(roi_x);
  Serial.print(", ");
  Serial.print(roi_y);
  
  int spad_n = distanceSensor.getSpadNb();
  Serial.print("\tSpadNb: ");
  Serial.print(spad_n);

  Serial.println();

  // battery level indication
  read_batt();
  delay(100);
}
