/*
  MLX90393 Magnetometer Example Code
  By: Nathan Seidle
  SparkFun Electronics
  Date: February 6th, 2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Read the mag fields on three XYZ axis

  Hardware Connections (Breakoutboard to Arduino):
  3.3V = 3.3V
  GND = GND
  SDA = A4
  SCL = A5

  Serial.print it out at 9600 baud to serial monitor.
*/

#include <Wire.h>
#include <MLX90393.h> //From https://github.com/tedyapo/arduino-MLX90393 by Theodore Yapo

//#define I2C_BASE_ADDR 0x0C

MLX90393 mlx;
MLX90393::txyz data; //Create a structure, called data, of four floats (t, x, y, and z)


void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Serial.println("MLX90393 Read Example");
  
  int status = mlx.begin(0,0,-1,Wire); 

  Serial.print("Init status: ");
  Serial.println(status);
}

void loop()
{
  int status = mlx.readData(data); //Read the values from the sensor
  
  Serial.print("magX[");
  Serial.print(data.x);
  Serial.print("] magY[");
  Serial.print(data.y);
  Serial.print("] magZ[");
  Serial.print(data.z);
  Serial.print("] temperature(C)[");
  Serial.print(data.t);
  Serial.print("]");

  Serial.println();

  delay(100);
}
