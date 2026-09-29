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
#include "FaceArt.h"
#endif
#if LESSON_SERVO
#include <Adafruit_PWMServoDriver.h>
#endif

namespace lesson {
bool streaming=false, testing=false, moving=false, prepared=false;
uint32_t started=0, lastSample=0, lastMotion=0, lastHealth=0, preparedAt=0;
int stage=-1;
bool animating=false;
uint32_t faceStarted=0, lastFrame=0;
int shownFrame=-1;
int requestedPulse=0, expressionShown=0;
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
  moving=streaming=testing=prepared=animating=false;
  requestedPulse=0;
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
  Serial.printf("Face: %s | animation: %s\n",faceName(expressionShown),animating ? "on" : "paused");
#endif
#if LESSON_SERVO
  report("PCA9685",driverReady,0x40);
  Serial.println("Motor lesson: fixed practice sweep. No calibration needed.");
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
  Serial.println("start  : readings every 100ms for 60 seconds (never starts a motor)");
  Serial.println("stop   : stop readings/test; request all motor pulses off where supported");
#if LESSON_SERVO
  Serial.println("test   : show the motor instructions; nothing moves yet");
  Serial.println("run    : after test + adult check, run channel 0 for 60 seconds");
  Serial.println("outputs: read all 16 output-off flags; only while stopped");
  Serial.println("Motor power must be off before upload/reset/opening monitor. stop is not power-off.");
#else
  Serial.println("test   : a 60-second observation experiment; 100ms rows; ends automatically");
#endif
#if LESSON_OLED
  Serial.println("awake | happy | sleep | curious | surprised | cool | cheeky | clear");
  Serial.println("Choose a 4-second animation, repeated for 60s; stop freezes it. No motor start.");
#if LESSON_KIND == 4
  Serial.println("test cycles all seven faces, four seconds each, for one minute.");
#endif
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
void renderFace(int frame) {
#if LESSON_OLED
  if (!displayReady) return;
  display.clearDisplay();
  if (expressionShown>=0 && expressionShown<FACE_COUNT) {
    const uint16_t index=pgm_read_word(&FACE_SEQUENCE[expressionShown][frame]);
    display.drawBitmap(0,0,FACE_BITMAPS[index],128,64,SSD1306_WHITE);
  }
  display.display();
#endif
  shownFrame=frame;
}
void draw(int expression) {
  expressionShown=expression;
  faceStarted=lastFrame=millis();
  animating=expression>=0 && displayReady;
  renderFace(0);
}
void animateFace(uint32_t now) {
#if LESSON_OLED
  if (!animating) return;
  if (finished(now,faceStarted,TEST_MS)) { animating=false; return; }
  // No delay or catch-up loop: serial, sampling and motor deadlines get a turn.
  if (finished(now,lastFrame,FACE_FRAME_MS)) {
    lastFrame=now;
    const int next=faceFrame(elapsed(now,faceStarted));
    if (next!=shownFrame) renderFace(next);
  }
#endif
}
void columns() {
#if LESSON_KIND == 0
  Serial.printf("%10s  %s\n","TIME_MS","MESSAGE");
#elif LESSON_KIND == 1 || LESSON_KIND == 2
  Serial.println(LESSON_KIND==1 ? "Acceleration in m/s2 (includes gravity). TIME_MS is board uptime." : "Rotation rate in deg/s (not angle). TIME_MS is board uptime.");
  Serial.printf("%10s %9s %9s %9s\n","TIME_MS","X","Y","Z");
#elif LESSON_KIND == 3
  Serial.println("Chip temperature, not calibrated room/body temperature.");
  Serial.printf("%10s %9s\n","TIME_MS","TEMP_C");
#elif LESSON_KIND == 4
  Serial.printf("%10s %9s\n","TIME_MS","DISPLAY");
#elif LESSON_KIND == 5
  Serial.println("Pulse request is NOT measured position; 0 means pulses requested off.");
  Serial.printf("%10s %9s %9s\n","TIME_MS","CHANNEL","PULSE_US");
#elif LESSON_KIND == 6
  Serial.println("A: m/s2 | G: deg/s | chip: C | pulse: us (requested, 0=off). TIME_MS: uptime.");
  Serial.printf("%10s %7s %7s %7s %7s %7s %7s %7s %8s\n","TIME_MS","AX","AY","AZ","GX","GY","GZ","TEMP_C","PULSE_US");
#endif
}
void read() {
  if(!ready()) { Serial.println("[X] Required device not ready. Type status."); return; }
  if(!sample()) return;
#if LESSON_KIND == 0
  Serial.printf("%10lu  Hello, teammate!\n",static_cast<unsigned long>(millis()));
#elif LESSON_KIND == 1
  Serial.printf("%10lu %9.2f %9.2f %9.2f\n",static_cast<unsigned long>(millis()),ax,ay,az);
#elif LESSON_KIND == 2
  Serial.printf("%10lu %9.2f %9.2f %9.2f\n",static_cast<unsigned long>(millis()),gx,gy,gz);
#elif LESSON_KIND == 3
  Serial.printf("%10lu %9.2f\n",static_cast<unsigned long>(millis()),temp);
#elif LESSON_KIND == 4
  const char *name=faceName(expressionShown);
  Serial.printf("%10lu %9s\n",static_cast<unsigned long>(millis()),name);
#elif LESSON_KIND == 5
  Serial.printf("%10lu %9d %9d\n",static_cast<unsigned long>(millis()),0,requestedPulse);
#elif LESSON_KIND == 6
  Serial.printf("%10lu %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %8d\n",static_cast<unsigned long>(millis()),ax,ay,az,gx,gy,gz,temp,requestedPulse);
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
  if(!strcmp(cmd,"stop")) {
    stop();
#if LESSON_SERVO
    Serial.println("Stopped. Ask an adult to unplug motor power. Nothing will restart by itself.");
#else
    Serial.println("Stopped. Type help for your next command.");
#endif
    return;
  }
  if(!strcmp(cmd,"help")) { help(); return; }
  if(!strcmp(cmd,"status")) { status(); return; }
  if(!strcmp(cmd,"scan")) { scan(); return; }
#if LESSON_SERVO
  if(!strcmp(cmd,"outputs")) { outputReport(); return; }
#endif
  if(moving) { Serial.println("The motor test is running. Type stop to end it now."); return; }
#if LESSON_SERVO
  if(!strcmp(cmd,"calibrate") || !strcmp(cmd,"more") || !strcmp(cmd,"back") || !strcmp(cmd,"keep") || !strncmp(cmd,"range",5)) {
    Serial.println("No calibration needed in this lesson. Type test, then follow the three steps."); return;
  }
#endif
  if(!strcmp(cmd,"read")) { columns(); read(); return; }
  if(!strcmp(cmd,"start")) { stop(); if(ready()) { streaming=true; started=lastSample=millis(); columns(); } else status(); return; }
  if(!strcmp(cmd,"test")) {
    stop(); if(!ready()) { status(); return; }
#if LESSON_SERVO
    prepared=true; preparedAt=millis();
    Serial.println("TRY THE MOVEMENT - nothing moves yet.");
    Serial.println("1. Keep the bare motor on connector 0. Keep hands clear.");
    Serial.println("2. Ask an adult to check it and turn on its 5V supply.");
    Serial.println("3. Type run and press Enter. Watch the motor slowly turn back and forth.");
    Serial.println("This is a wider practice movement, not a full-travel test.");
    Serial.println("It stops after one minute. Type stop to stop sooner.");
    Serial.println("If it sticks or keeps buzzing, type stop and unplug motor power.");
#else
    testing=streaming=true; started=lastSample=millis(); stage=-1;
    Serial.println("60-second test started; a row every 100ms. stop ends early.");
    columns();
#endif
    return;
  }
#if LESSON_SERVO
  if(!strcmp(cmd,"run")) {
    if(!prepared || finished(millis(),preparedAt,60000) || !ready()) {
      prepared=false; Serial.println("Type test first, then follow its three steps. Nothing moved."); return;
    }
    // Recheck now: initialization flags may be stale since preparation.
    if(!present() || !sample()) { fault("[FAULT] Pre-run device check failed. No motor start; inspect with power off."); return; }
    stop(); if(!outputsOff() || !ready()) return;
    if(driver.setPWM(0,0,307)!=0) { driverReady=false; fault("[FAULT] Motor write failed. Unplug power."); return; }
    requestedPulse=1500; moving=true; started=lastSample=millis(); stage=-1;
    Serial.println("RUNNING: one minute. Watch the motor; no more typing is needed.");
    Serial.println("Type stop to finish early. A normal finish prints DONE; an error prints FAULT."); columns();
    return;
  }
#endif
#if LESSON_OLED
  const char *faceCommands[]={"awake","happy","sleep","curious","surprised","cool","cheeky"};
  int chosen=-2;
  for(int i=0;i<FACE_COUNT;i++) if(!strcmp(cmd,faceCommands[i])) chosen=i;
  if(!strcmp(cmd,"clear")) chosen=-1;
  if(chosen!=-2) {
    testing=prepared=false;
    draw(chosen);
    Serial.printf("Face: %s. Check the cartoon eyes and mouth on the screen.\n",faceName(chosen)); return;
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
  if(displayReady) { draw(0); animating=false; }
#endif
  Serial.println("[BOOT] Robot lessons: practice sweep edition. Ready for commands.");
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
  if(prepared && finished(now,preparedAt,60000)) {
    Serial.println("READY CHECK TIMED OUT. Nothing moved. Type test when you are ready.");
    prepared=false;
  }
  if((moving||streaming||testing||animating) && finished(now,lastHealth,250)) {
    lastHealth=now;
    if(!present()) {
      fault("[FAULT] Required device stopped answering. Remove motor power if used; inspect with power off.");
      imuReady=displayReady=driverReady=false;
    }
  }
#if LESSON_SERVO
  if(moving) {
    const uint32_t age=elapsed(now,started);
    if(age>=TEST_MS) {
      stop();
      Serial.println("DONE: the one-minute test finished normally. Motor signals are off.");
      Serial.println("Ask the adult to unplug motor power. Tell your teammate what moved.");
      Serial.println("The board is still ready. Type test to try again, or status to check it.");
    }
    else {
      if(int(age/1000)!=stage) {
        stage=age/1000;
        if(stage%10==0) Serial.printf("RUNNING: %d seconds left.\n",60-stage);
#if LESSON_OLED
        const int nextFace=age>=56000 ? 2 : (stage/4)%FACE_COUNT;
        if(expressionShown!=nextFace || !animating) draw(nextFace);
#endif
      }
      if(finished(now,lastMotion,20)) {
        lastMotion=now;
        requestedPulse=practicePulse(age);
        const uint16_t ticks=(requestedPulse*4096L+10000)/20000;
        if(driver.setPWM(0,0,ticks)!=0) { fault("[FAULT] Servo write failed. Unplug motor power."); driverReady=false; }

      }
    }
  }
#endif
  if(streaming && finished(now,started,TEST_MS)) { stop(); Serial.println("DONE: 60-second test ended. Record your observation; no automatic pass."); }
#if LESSON_OLED
  if(testing) {
    int next=elapsed(now,started)/FACE_LOOP_MS;
    if(next!=stage) { stage=next; draw(next%FACE_COUNT); }
  }
#endif
  if((streaming || moving) && finished(now,lastSample,SAMPLE_MS)) {
    lastSample=now;
    read();
  }
  animateFace(millis());
}
}
