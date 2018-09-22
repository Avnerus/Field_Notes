#include "pumpng.h"
#include "valve.h"

PumpNg pump(22,21,23,20);
Valve valve(15,16,17,20);


void setup() {
  Serial.begin(9600);
  Serial.println("Pumpng test");
  pump.init();
  valve.init();
}


void loop() {

  valve.close();
  pump.inflate();
  delay(3000);
  valve.open();
  pump.stop();
  delay(2000);
}
