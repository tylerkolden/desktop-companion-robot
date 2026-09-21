#include <Wire.h>
// Connect only the lesson's OLED or IMU; change wiring with power off.
void setup() {
  Serial.begin(115200);
  Wire.begin(6, 7); // XIAO D4=SDA, D5=SCL. These numbers are GPIO numbers.
  Wire.setClock(100000); Wire.setTimeOut(25);
}
void loop() {
  Serial.println("Scanner lesson: looking for I2C acknowledgements");
  int found = 0;
  for (int address = 1; address < 127; ++address) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) { Serial.printf("Found 0x%02X\n", address); ++found; }
  }
  Serial.printf("%d responding addresses; not a complete device test.\n", found);
  delay(3000);
}
