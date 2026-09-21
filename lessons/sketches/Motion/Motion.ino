#include <Wire.h>
#include <Adafruit_MPU6050.h>
// XIAO + GY-521 only. Measurements do not command movement.
Adafruit_MPU6050 imu;
bool ready = false;
unsigned long lastSample = 0;
void setup() {
  Serial.begin(115200);
  Wire.begin(6, 7); Wire.setTimeOut(25);
  ready = imu.begin(0x68, &Wire) || imu.begin(0x69, &Wire);
  if (ready) {
    imu.setAccelerometerRange(MPU6050_RANGE_8_G);
    imu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  }
}
void loop() {
  unsigned long now = millis();
  if (now - lastSample < 500) return;
  lastSample = now;
  if (!ready) { Serial.println("IMU not ready. Power off, inspect wiring, try Scanner."); return; }
  sensors_event_t a, g, t;
  if (!imu.getEvent(&a, &g, &t)) { Serial.println("Sensor read failed"); return; }
  Serial.printf("Acceleration m/s2: x=%.2f y=%.2f z=%.2f\n",
                a.acceleration.x, a.acceleration.y, a.acceleration.z);
}
