#pragma once
// Each .ino selects only the hardware it needs BEFORE including this file.
// No Companion state, saved calibration or automatic motion is used here.
#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include "LessonLogic.h"
#if LESSON_IMU
#include <Adafruit_MPU6050.h>
#endif
#if LESSON_OLED
#include <Adafruit_SSD1306.h>
#endif
#if LESSON_SERVO
#include <Adafruit_PWMServoDriver.h>
#endif

namespace lesson {
bool streaming=false, testing=false, moving=false, prepared=false;
uint32_t started=0, lastSample=0, lastMotion=0, lastHealth=0, preparedAt=0;
int stage=-1;
char line[64]; size_t used=0; bool overflow=false;
float ax=0,ay=0,az=0,gx=0,gy=0,gz=0,temp=0;
uint8_t imuAddress=0, displayAddress=0;
bool imuReady=false, displayReady=false, driverReady=false;
#if LESSON_IMU
Adafruit_MPU6050 imu;
#endif
#if LESSON_OLED
Adafruit_SSD1306 display(128,64,&Wire,-1,400000,100000);
#endif
#if LESSON_SERVO
Adafruit_PWMServoDriver driver(0x40,Wire);
#endif

bool probe(uint8_t address) { Wire.beginTransmission(address); return Wire.endTransmission()==0; }
bool outputsOff() {
  bool ok=true;
#if LESSON_SERVO
  if (driverReady) {
    for (int ch=0;ch<16;ch++) if (driver.setPWM(ch,0,4096)!=0) ok=false;
    if (!ok) { driverReady=false; Serial.println("[FAULT] Output-off write failed. Unplug motor power."); }
  }
#endif
  return ok;
}
void stop() {
  moving=streaming=testing=prepared=false;
  outputsOff();
}
bool ready() {
  return (!LESSON_IMU || imuReady) && (!LESSON_OLED || displayReady) && (!LESSON_SERVO || driverReady);
}
bool present() {
  return (!LESSON_IMU || probe(imuAddress)) && (!LESSON_OLED || probe(displayAddress)) && (!LESSON_SERVO || probe(0x40));
}
void report(const char *name, bool initialized, uint8_t address) {
  Serial.printf("%s %s at 0x%02X: %s\n", initialized ? "[OK]" : "[X]",name,address,
    initialized ? "initialized; test its behavior next" : "not ready; stop and check this lesson's wiring");
}
void status() {
  Serial.printf("\nLESSON: %s | ESP32 answering\n",LESSON_NAME);
#if LESSON_IMU
  report("MPU6050",imuReady,imuAddress ? imuAddress : 0x68);
#endif
#if LESSON_OLED
  report("OLED",displayReady,displayAddress ? displayAddress : 0x3c);
#endif
#if LESSON_SERVO
  report("PCA9685",driverReady,0x40);
  Serial.printf("Motor sequence: %s | prepared: %s | channel 0 only\n", moving ? "RUNNING" : "off",prepared ? "yes (60s expiry)" : "no");
#endif
  Serial.printf("Readings: %s | timed test: %s\n",streaming ? "on" : "off",testing ? "on" : "off");
  Serial.println("Readiness is not a fresh scan or a physical pass. Record what YOU observe.");
}
void help() {
  Serial.printf("\n%s -- type one command, then Enter\n",LESSON_NAME);
  Serial.println("help   : show this menu again");
  Serial.println("status : show only the devices needed for THIS lesson");
  Serial.println("scan   : fresh I2C addresses; stops any test first");
  Serial.println("read   : one reading/report (never starts a motor)");
  Serial.println("start  : repeat readings every 500ms (never starts a motor)");
  Serial.println("stop   : stop readings/test; request all motor pulses off where supported");
#if LESSON_SERVO
  Serial.println("test   : stop and print motor preparation; no movement");
  Serial.println("run    : after test + adult check, run channel 0 for 8 seconds");
  Serial.println("outputs: read all 16 output-off flags; only while stopped");
  Serial.println("Motor power must be off before upload/reset/opening monitor. stop is not power-off.");
#else
  Serial.println("test   : a 10-second observation experiment; ends automatically");
#endif
#if LESSON_OLED
  Serial.println("awake | happy | sleep | clear : choose what the screen draws");
#endif
  Serial.println("Ctrl+C closes the COMPUTER monitor; it does not send stop to the board.");
}
void fault(const char *message) { stop(); Serial.println(message); }
bool sample() {
#if LESSON_IMU
  if (!imuReady) return false;
  sensors_event_t a,g,t;
  if (!probe(imuAddress) || !imu.getEvent(&a,&g,&t)) {
    imuReady=false; fault("[FAULT] Sensor lost. Power off, inspect wiring, reboot."); return false;
  }
  ax=a.acceleration.x; ay=a.acceleration.y; az=a.acceleration.z;
  gx=degreesPerSecond(g.gyro.x); gy=degreesPerSecond(g.gyro.y); gz=degreesPerSecond(g.gyro.z); temp=t.temperature;
  if (!isfinite(ax)||!isfinite(ay)||!isfinite(az)||!isfinite(gx)||!isfinite(gy)||!isfinite(gz)||!isfinite(temp)) {
    imuReady=false; fault("[FAULT] Invalid sensor reading. Test stopped."); return false;
  }
#endif
  return true;
}
void draw(int expression) {
#if LESSON_OLED
  if (!displayReady) return;
  display.clearDisplay();
  if (expression>=0) {
    int h=expression==2 ? 4 : 24;
    for(int x : {25,79}) {
      display.fillRoundRect(x,22,24,h,3,SSD1306_WHITE);
      if(expression==1) display.fillRect(x,34,24,12,SSD1306_BLACK);
    }
    display.setTextSize(1); display.setTextColor(SSD1306_WHITE); display.setCursor(0,54);
    display.print(expression==0 ? "AWAKE" : expression==1 ? "HAPPY" : "SLEEP");
  }
  display.display();
#else
  (void)expression;
#endif
}
void read() {
  if(!ready()) { Serial.println("[X] Required device not ready. Type status."); return; }
  if(!sample()) return;
#if LESSON_KIND == 0
  Serial.printf("Hello, teammate! Uptime %lu seconds.\n",static_cast<unsigned long>(millis()/1000));
#elif LESSON_KIND == 1
  Serial.printf("Acceleration m/s2: X=%.2f Y=%.2f Z=%.2f (includes gravity)\n",ax,ay,az);
#elif LESSON_KIND == 2
  Serial.printf("Rotation rate deg/s: X=%.2f Y=%.2f Z=%.2f (rate, NOT angle)\n",gx,gy,gz);
#elif LESSON_KIND == 3
  Serial.printf("Chip temperature: %.2f C (not a room/body thermometer)\n",temp);
#elif LESSON_KIND == 4
  Serial.println("OLED initialized. Use awake, happy, sleep, clear or test; observe the actual screen.");
#elif LESSON_KIND == 5
  Serial.println("Driver initialized; pulse targets are not measured shaft positions. test explains the movement check.");
#elif LESSON_KIND == 6
  Serial.printf("Accel m/s2 %.2f %.2f %.2f | Gyro deg/s %.2f %.2f %.2f | Chip %.2f C\n",ax,ay,az,gx,gy,gz,temp);
#endif
}
void scan() {
  stop(); Serial.println("Fresh scan (tests stopped):");
  int count=0;
  for(uint8_t a=1;a<127;a++) if(probe(a)) { Serial.printf("  0x%02X answered\n",a); count++; }
  Serial.printf("%d addresses. Address replies do not prove full operation.\n",count);
}
void outputReport() {
#if LESSON_SERVO
  if(moving) { Serial.println("Type stop before outputs."); return; }
  if(!driverReady) { Serial.println("Driver unavailable."); return; }
  for(int ch=0;ch<16;ch++) {
    Wire.beginTransmission(0x40); Wire.write(0x09+4*ch);
    if(Wire.endTransmission(false)!=0 || Wire.requestFrom(uint8_t(0x40),uint8_t(1))!=1) {
      fault("[FAULT] Readback failed. Unplug motor power."); return;
    }
    Serial.printf("Channel %02d full-off=%d\n",ch,(Wire.read()&0x10)!=0);
  }
  Serial.println("Flags describe driver registers, not motor voltage or actual shaft position.");
#endif
}
void command(const char *cmd) {
  if(!strcmp(cmd,"stop")) { stop(); Serial.println("Stopped. No automatic restart."); return; }
  if(!strcmp(cmd,"help")) { help(); return; }
  if(!strcmp(cmd,"status")) { status(); return; }
  if(!strcmp(cmd,"scan")) { scan(); return; }
#if LESSON_SERVO
  if(!strcmp(cmd,"outputs")) { outputReport(); return; }
#endif
  if(moving) { Serial.println("Motor running. stop cancels; status reports progress."); return; }
  if(!strcmp(cmd,"read")) { read(); return; }
  if(!strcmp(cmd,"start")) { stop(); if(ready()) streaming=true; else status(); return; }
  if(!strcmp(cmd,"test")) {
    stop(); if(!ready()) { status(); return; }
#if LESSON_SERVO
    prepared=true; preparedAt=millis();
    Serial.println("PREPARE: one unloaded SG90, channel 0; no horn/leg; shaft clear.");
    Serial.println("Follow this lesson's adult wiring and 5V power check. Guide starts NO movement.");
    Serial.println("Fixed 1450-1550us bench range, center 1500us. Saved Companion calibration is NOT used.");
    Serial.println("Center -> three low/high pairs -> center -> OFF at 8s. stop cancels.");
    Serial.println("Only after preparation, connect motor power and type run within 60 seconds.");
#else
    testing=streaming=true; started=millis(); stage=-1;
    Serial.println("10-second test started. Observe, predict, record. stop ends early.");
#endif
    return;
  }
#if LESSON_SERVO
  if(!strcmp(cmd,"run")) {
    if(!prepared || finished(millis(),preparedAt,60000) || !ready()) {
      prepared=false; Serial.println("Not prepared. Type test and complete its checks first."); return;
    }
    // Recheck now: initialization flags may be stale since preparation.
    if(!present() || !sample()) { fault("[FAULT] Pre-run device check failed. No motor start; inspect with power off."); return; }
    stop(); if(!outputsOff() || !ready()) return;
    if(driver.setPWM(0,0,307)!=0) { driverReady=false; fault("[FAULT] Motor write failed. Unplug power."); return; }
    moving=true; started=millis(); stage=-1;
    Serial.println("RUN: channel 0 only. Watch the shaft; stop cancels."); return;
  }
#endif
#if LESSON_OLED
  if(!strcmp(cmd,"awake")||!strcmp(cmd,"happy")||!strcmp(cmd,"sleep")||!strcmp(cmd,"clear")) {
    testing=streaming=prepared=false;
    draw(!strcmp(cmd,"clear") ? -1 : !strcmp(cmd,"awake") ? 0 : !strcmp(cmd,"happy") ? 1 : 2);
    Serial.println("Drawing requested. Check the screen with your eyes."); return;
  }
#endif
  prepared=false;
  Serial.println("Unknown command. Type help. Use one word, then Enter.");
}
void begin() {
  Serial.begin(115200); Wire.begin(6,7); Wire.setClock(100000); Wire.setTimeOut(25);
#if LESSON_SERVO
  driverReady=probe(0x40)&&driver.begin();
  if(driverReady && outputsOff()) { driver.setPWMFreq(50); outputsOff(); }
#endif
#if LESSON_IMU
  imuAddress=probe(0x68) ? 0x68 : probe(0x69) ? 0x69 : 0;
  imuReady=imuAddress && imu.begin(imuAddress,&Wire);
  if(imuReady) { imu.setAccelerometerRange(MPU6050_RANGE_8_G); imu.setGyroRange(MPU6050_RANGE_250_DEG); imu.setFilterBandwidth(MPU6050_BAND_21_HZ); }
#endif
#if LESSON_OLED
  displayAddress=probe(0x3c) ? 0x3c : probe(0x3d) ? 0x3d : 0;
  displayReady=displayAddress && display.begin(SSD1306_SWITCHCAPVCC,displayAddress,true,false);
  if(displayReady) draw(0);
#endif
  help(); status();
}
void tick() {
  for(int budget=0;budget<64 && Serial.available();budget++) {
    char c=Serial.read();
    if(c=='\r') continue;
    if(c=='\n') {
      if(!overflow) { line[used]=0; if(used) command(line); }
      else Serial.println("Input too long; discarded. Test stopped. Type help.");
      used=0; overflow=false;
    } else if(!overflow) {
      if(used<sizeof(line)-1) line[used++]=c;
      else { overflow=true; stop(); }
    }
  }
  const uint32_t now=millis();
  if(prepared && finished(now,preparedAt,60000)) { prepared=false; Serial.println("Preparation expired. Type test again."); }
  if((moving||streaming||testing) && finished(now,lastHealth,250)) {
    lastHealth=now;
    if(!present()) {
      fault("[FAULT] Required device stopped answering. Remove motor power if used; inspect with power off.");
      imuReady=displayReady=driverReady=false;
    }
  }
#if LESSON_SERVO
  if(moving) {
    const uint32_t age=elapsed(now,started);
    if(age>=8000) { stop(); Serial.println("DONE: pulses requested OFF. Unplug motor supply. Observe YES / NO / NOT SURE."); }
    else {
      if(int(age/1000)!=stage) {
        stage=age/1000;
        Serial.printf("Step %d/8: %s\n",stage+1,stage==0||stage==7 ? "CENTER" : stage%2 ? "LOW" : "HIGH");
#if LESSON_OLED
        draw(stage==7 ? 2 : stage%2 ? 1 : 0);
#endif
      }
      if(finished(now,lastMotion,20)) {
        lastMotion=now;
        const uint16_t ticks=(motorPulse(age)*4096L+10000)/20000;
        if(driver.setPWM(0,0,ticks)!=0) { fault("[FAULT] Servo write failed. Unplug motor power."); driverReady=false; }
      }
    }
  }
#endif
  if(testing && finished(now,started,10000)) { stop(); Serial.println("DONE: test ended. Record your observation; no automatic pass."); }
#if LESSON_OLED
  if(testing) {
    int next=elapsed(now,started)/2000;
    if(next!=stage) { stage=next; draw(next%3); Serial.println("Expression changed: observe the screen."); }
  }
#endif
  if(streaming && finished(now,lastSample,500)) { lastSample=now; read(); }
#if LESSON_KIND == 6
  if(moving && finished(now,lastSample,500)) { lastSample=now; read(); }
#endif
}
}
