#include <stdint.h>
#include <Wire.h>

#define I2C_ADDR  2

void setup() {
  // set up addr, r/w events, etc.
  Wire.begin(I2C_ADDR);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent); 
  Serial.begin(9600);
}

void loop() {
}

void receiveEvent(int numBytes) {
  // read data when there's something to read
  while (Wire.available() > 0)
  {
    char receive  = Wire.read();
    Serial.print(receive);
  }
}

void requestEvent() {
  // write 'data' on request
  Serial.write("data");
}
