
/*
  Reading CO2, from SCD30 in Wemos D1 and sendin it via MIDI Networking.
*/

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include "AppleMidi.h"
#include "SparkFun_SCD30_Arduino_Library.h" 

char ssid[] = "FabLabNet"; //  your network SSID (name)
char pass[] = "*********";    // your network password (use for WPA, or use as key for WEP)

unsigned long t0 = millis();
bool isConnected = false;

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h

// Forward declaration
void OnAppleMidiConnected(uint32_t ssrc, char* name);
void OnAppleMidiDisconnected(uint32_t ssrc);
void OnAppleMidiNoteOn(byte channel, byte note, byte velocity);
void OnAppleMidiNoteOff(byte channel, byte note, byte velocity);


SCD30 airSensor;

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  Serial.println("SCD30 Example");

  airSensor.begin(); //This will cause readings to occur every two seconds
    Serial.print(F("Getting IP address..."));

    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(F("."));
    }
    Serial.println(F(""));
    Serial.println(F("WiFi connected"));


    Serial.println();
    Serial.print(F("IP address is "));
    Serial.println(WiFi.localIP());

    Serial.println(F("OK, now make sure you an rtpMIDI session that is Enabled"));
    Serial.print(F("Add device named Arduino with Host/Port "));
    Serial.print(WiFi.localIP());
    Serial.println(F(":5004"));
    Serial.println(F("Then press the Connect button"));
    Serial.println(F("Then open a MIDI listener (eg MIDI-OX) and monitor incoming notes"));

    // Create a session and wait for a remote host to connect to us
    AppleMIDI.begin("test");

    AppleMIDI.OnConnected(OnAppleMidiConnected);
    AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);

    AppleMIDI.OnReceiveNoteOn(OnAppleMidiNoteOn);
    AppleMIDI.OnReceiveNoteOff(OnAppleMidiNoteOff);
}

void loop()
{
    // Listen to incoming notes
    AppleMIDI.run();



  if (airSensor.dataAvailable())
  {
    int co2Value = airSensor.getCO2();
    Serial.print("co2(ppm):");
    Serial.print(co2Value);

    Serial.print(" temp(C):");
    Serial.print(airSensor.getTemperature(), 1);

    Serial.print(" humidity(%):");
    Serial.print(airSensor.getHumidity(), 1);

    Serial.println();

    if (isConnected && (millis() - t0) > 1000)
    {

      byte controlNumber = 1;
      byte controlValue = (((float)co2Value - 700.0) / 500.0) * 127;
      byte channel = 1;

      Serial.print("Sending MIDI ");
      Serial.println(controlValue);


      AppleMIDI.controlChange(controlNumber, controlValue, channel);

      /*
      AppleMIDI.noteOn(note, velocity, channel);
      AppleMIDI.noteOff(note, velocity, channel);
      */
    }
  }
  else
    Serial.println("No data");

  delay(1000);
}

void OnAppleMidiConnected(uint32_t ssrc, char* name) {
  isConnected  = true;
  Serial.print(F("Connected to session "));
  Serial.println(name);
}

// -----------------------------------------------------------------------------
// rtpMIDI session. Device disconnected
// -----------------------------------------------------------------------------
void OnAppleMidiDisconnected(uint32_t ssrc) {
  isConnected  = false;
  Serial.println(F("Disconnected"));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OnAppleMidiNoteOn(byte channel, byte note, byte velocity) {
}
void OnAppleMidiNoteOff(byte channel, byte note, byte velocity) {
}

