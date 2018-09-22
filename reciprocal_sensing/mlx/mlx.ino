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
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <ESP8266WiFi.h>

//#define I2C_BASE_ADDR 0x0C

MLX90393 mlx;
MLX90393::txyz data; //Create a structure, called data, of four floats (t, x, y, and z)
const char* ssid     = "FabLabNet";         
const char* password = "Betonprint";     
const IPAddress outIp(192,168,0,196);
const unsigned int outPort = 9000;
WiFiUDP Udp;


void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Serial.println("MLX90393 Read Example");
  WiFi.begin(ssid, password);
    Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;


  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); 
  
  int status = mlx.begin(0,0,-1,Wire); 

  Serial.print("Init status: ");
  Serial.println(status);
}

void loop() {

    int status = mlx.readData(data); //Read the values from the sensor

    //Serial.print("magX[");
    Serial.print('(');
    Serial.print(data.x);
    Serial.print(',');
    //Serial.print("] magY[");
    Serial.print(data.y);
    // Serial.print("] magZ[");
    Serial.print(',');
    Serial.print(data.z);
    Serial.print(')');
    /*
    Serial.print("] temperature(C)[");
    Serial.print(data.t);
    Serial.print("]");
    */

    Serial.println();

    OSCMessage msgX("/magX");
    msgX.add (data.x);
    Udp.beginPacket(outIp, outPort);
    msgX.send(Udp); 
    Udp.endPacket();
    msgX.empty();

    OSCMessage msgY("/magY");
    msgY.add (data.y);
    Udp.beginPacket(outIp, outPort);
    msgY.send(Udp); 
    Udp.endPacket();
    msgY.empty();

    OSCMessage msgZ("/magZ");
    msgZ.add (data.z);
    Udp.beginPacket(outIp, outPort);
    msgZ.send(Udp); 
    Udp.endPacket();
    msgZ.empty();
}
