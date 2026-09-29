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
bool calibrationPrepared=false, calibrating=false, rangeChecked=false;
bool lowMarked=false, highMarked=false;
int sweepLow=1450, sweepHigh=1550, draftLow=1450, draftHigh=1550, calibrationTarget=1500;
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
  calibrating=calibrationPrepared=false;
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
  Serial.printf("Sweep: %d..%dus | checked=%s | endpoint marks: low=%s high=%s\n",sweepLow,sweepHigh,rangeChecked ? "yes" : "no",lowMarked ? "yes" : "no",highMarked ? "yes" : "no");
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
  Serial.println("calibrate : prepare endpoint teaching; then run starts at center");
  Serial.println("lower | higher | center | marklow | markhigh | finish : during calibration");
  Serial.println("range LOW HIGH : load endpoints YOU already checked (microseconds)");
  Serial.println("range reset : forget endpoints before changing servo or mechanism");
  Serial.println("test   : prepare full checked-range sweep; no movement yet");
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
  if(!strcmp(cmd,"stop")) { stop(); Serial.println("Stopped. No automatic restart."); return; }
  if(!strcmp(cmd,"help")) { help(); return; }
  if(!strcmp(cmd,"status")) { status(); return; }
  if(!strcmp(cmd,"scan")) { scan(); return; }
#if LESSON_SERVO
  if(!strcmp(cmd,"outputs")) { outputReport(); return; }
#endif
#if LESSON_SERVO
  if(calibrating) {
    if(!strcmp(cmd,"lower") || !strcmp(cmd,"higher") || !strcmp(cmd,"center")) {
      if(requestedPulse!=calibrationTarget) { Serial.println("Still moving slowly. Wait before the next step."); return; }
      int next=!strcmp(cmd,"center") ? 1500 : calibrationTarget+(!strcmp(cmd,"lower") ? -25 : 25);
      if(next<500 || next>2500) { Serial.println("Software ceiling reached. Do not force more travel."); return; }
      calibrationTarget=next;
      Serial.printf("Moving gently toward %dus. Watch the shaft; stop if binding or buzzing.\n",next); return;
    }
    if(!strcmp(cmd,"marklow") || !strcmp(cmd,"markhigh")) {
      if(requestedPulse!=calibrationTarget) { Serial.println("Wait for the step to finish before marking."); return; }
      if(!strcmp(cmd,"marklow") && requestedPulse<=1450) { draftLow=requestedPulse; lowMarked=true; }
      else if(!strcmp(cmd,"markhigh") && requestedPulse>=1550) { draftHigh=requestedPulse; highMarked=true; }
      else { Serial.println("Low must be below center (<=1450); high above center (>=1550)."); return; }
      Serial.printf("Recorded %dus as observed usable. Both marks + finish enable the sweep.\n",requestedPulse); return;
    }
    if(!strcmp(cmd,"finish")) {
      const bool checked=lowMarked && highMarked && validRange(draftLow,draftHigh);
      stop();
      if(!checked) { Serial.println("Stopped; both endpoint marks are needed. calibrate then run to continue."); return; }
      sweepLow=draftLow; sweepHigh=draftHigh; rangeChecked=true;
      Serial.printf("Checked range stored for THIS session/channel 0: %d..%dus.\n",sweepLow,sweepHigh);
      Serial.printf("Notebook/reload command for this same servo: range %d %d\n",sweepLow,sweepHigh);
      Serial.println("Unplug motor power. Use test, adult check, then run for the sweep."); return;
    }
    Serial.println("Calibration: lower / higher / center / marklow / markhigh / finish / stop."); return;
  }
#endif
  if(moving) { Serial.println("Motor running. stop cancels; status reports progress."); return; }
#if LESSON_SERVO
  if(!strcmp(cmd,"range reset")) {
    stop(); rangeChecked=lowMarked=highMarked=false; sweepLow=draftLow=1450; sweepHigh=draftHigh=1550;
    Serial.println("Endpoints forgotten. Calibrate this unloaded servo before a sweep."); return;
  }
  if(!strncmp(cmd,"range ",6)) {
    stop();
    char lowText[6], highText[6], extra;
    if(sscanf(cmd,"range %5s %5s %c",lowText,highText,&extra)!=2) { Serial.println("Use range LOW HIGH with previously checked microseconds."); return; }
    char *lowEnd, *highEnd;
    long low=strtol(lowText,&lowEnd,10), high=strtol(highText,&highEnd,10);
    if(*lowEnd || *highEnd || !validRange(low,high)) { Serial.println("Invalid endpoints. Software envelope 500..2500; low<=1450, high>=1550. This is NOT a safe-range recommendation."); return; }
    sweepLow=low; sweepHigh=high; rangeChecked=true;
    draftLow=low; draftHigh=high; lowMarked=highMarked=true;
    Serial.printf("Using your checked endpoints %d..%dus; no motion. test prepares the sweep.\n",sweepLow,sweepHigh); return;
  }
  if(!strcmp(cmd,"calibrate")) {
    stop(); if(!ready()) { status(); return; }
    rangeChecked=false; prepared=calibrationPrepared=true; preparedAt=millis();
    Serial.println("CALIBRATION: unloaded channel-0 servo, shaft clear. No movement yet.");
    Serial.println("After lesson power check, run within 60s starts at 1500us; initial centering may move.");
    Serial.println("lower/higher requests one 25us step; center returns gradually. Never push against a stop.");
    Serial.println("Mark comfortably usable endpoints with marklow/markhigh; finish stops and accepts both.");
    Serial.println("60s automatic pulse-off; marks survive stop so another calibrate/run can continue."); return;
  }
#endif
  if(!strcmp(cmd,"read")) { columns(); read(); return; }
  if(!strcmp(cmd,"start")) { stop(); if(ready()) { streaming=true; started=lastSample=millis(); columns(); } else status(); return; }
  if(!strcmp(cmd,"test")) {
    stop(); if(!ready()) { status(); return; }
#if LESSON_SERVO
    if(!rangeChecked) { Serial.println("No checked travel range. Type calibrate first, or range LOW HIGH from your notebook for this servo."); return; }
    prepared=true; preparedAt=millis();
    Serial.println("PREPARE: one unloaded SG90, channel 0; no horn/leg; shaft clear.");
    Serial.println("Follow this lesson's adult wiring and 5V power check. Guide starts NO movement.");
    Serial.printf("Full CHECKED range %d..%dus, center 1500us. No inferred angles or mechanical stop detection.\n",sweepLow,sweepHigh);
    Serial.println("Seven slow center/low/high/center sweeps (56s), center rest (4s), OFF at 60s. stop cancels.");
    Serial.println("Only after preparation, connect motor power and type run within 60 seconds.");
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
      prepared=false; Serial.println("Not prepared. Type test and complete its checks first."); return;
    }
    // Recheck now: initialization flags may be stale since preparation.
    if(!present() || !sample()) { fault("[FAULT] Pre-run device check failed. No motor start; inspect with power off."); return; }
    const bool beginCalibration=calibrationPrepared;
    stop(); if(!outputsOff() || !ready()) return;
    if(driver.setPWM(0,0,307)!=0) { driverReady=false; fault("[FAULT] Motor write failed. Unplug power."); return; }
    calibrating=beginCalibration; calibrationTarget=1500;
    requestedPulse=1500; moving=true; started=lastSample=millis(); stage=-1;
    Serial.println(calibrating ? "CALIBRATING: 60 seconds. lower/higher/center; marklow/markhigh; finish. stop cancels." : "SWEEP: 60 seconds through your checked limits, channel 0. stop cancels."); columns(); return;
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
    if(age>=TEST_MS) { stop(); Serial.println("DONE: 60 seconds; pulses requested OFF. Unplug motor supply. Observe YES / NO / NOT SURE."); }
    else {
      if(int(age/1000)!=stage) {
        stage=age/1000;
#if LESSON_OLED
        const int nextFace=age>=56000 ? 2 : (stage/4)%FACE_COUNT;
        if(!calibrating && (expressionShown!=nextFace || !animating)) draw(nextFace);
#endif
      }
      if(finished(now,lastMotion,20)) {
        lastMotion=now;
        requestedPulse=calibrating ? approach(requestedPulse,calibrationTarget) : sweepPulse(age,sweepLow,sweepHigh);
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
  if((streaming || moving) && finished(now,lastSample,SAMPLE_MS)) { lastSample=now; read(); }
  animateFace(millis());
}
}
