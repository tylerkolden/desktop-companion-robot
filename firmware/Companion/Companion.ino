// Original bench-first firmware. Read docs/FIRST-POWER-ON.md before attaching legs.
#include <Arduino.h>
#include <Wire.h>
#include <Preferences.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_PWMServoDriver.h>
#include "Logic.h"
#include "LogicTests.h"
#include "Face.h"

constexpr uint8_t PCA_ADDRESS = 0x40;
constexpr uint32_t CAL_TIMEOUT = 15000;
constexpr uint32_t IDLE_TIMEOUT = 30000;
struct Calibration { int low = 1450; int center = 1500; int high = 1550; int direction = 1; bool saved = false; };
Calibration calibration[4];
// Use the display library's 400kHz transfer rate, restoring 100kHz afterwards.
// A full framebuffer at 100kHz would monopolize the loop for roughly 90ms.
Adafruit_SSD1306 screen(128, 64, &Wire, -1, 400000, 100000);
Adafruit_MPU6050 imu;
Adafruit_PWMServoDriver pwm(PCA_ADDRESS, Wire);
Preferences preferences;
companion::TapDetector taps;
uint8_t oledAddress = 0, imuAddress = 0;
bool pcaOK = false, oledOK = false, imuOK = false, storageOK = false;
bool upright = false, imuSampleValid = false, demo = false;
bool benchTest = false;
bool benchOffsetSent = false;
uint32_t benchStart = 0;
int activeChannel = -1;
int currentPulse = 1500;
uint32_t lastCommand = 0, lastInteraction = 0, lastMotion = 0, demoStart = 0;
uint32_t lastSensor = 0, lastFace = 0, lastTelemetry = 0, lastHealth = 0;
float ax = 0, ay = 0, az = 0;
Mood mood = Mood::Awake;
char input[96];
size_t used = 0;
bool overflowed = false;

bool probe(uint8_t address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}

void stopMotion() {
  demo = false;
  benchTest = false;
  activeChannel = -1;
  taps = companion::TapDetector{};
  if (pcaOK) {
    for (uint8_t ch = 0; ch < 16; ++ch) pwm.setPWM(ch, 0, 4096);
  }
}

void scanBus() {
  stopMotion();
  bool found[127] = {};
  Serial.println("\n--- Device roll call: who answers now? ---");
  Serial.println("Control pulses were requested OFF before this scan.");
  for (uint8_t address = 1; address < 127; ++address) found[address] = probe(address);
  const uint8_t addresses[] = {0x3C, 0x3D, 0x68, 0x69, 0x40};
  const char *names[] = {"OLED screen", "OLED screen (alternate)", "Motion sensor", "Motion sensor (alternate)", "Motor driver"};
  for (int i = 0; i < 5; ++i) {
    if (found[addresses[i]]) Serial.printf("[OK] %-26s 0x%02X - answered!\n", names[i], addresses[i]);
  }
  if (!found[0x3C] && !found[0x3D]) Serial.println("[X]  OLED screen    0x3C / 0x3D - no answer");
  if (!found[0x68] && !found[0x69]) Serial.println("[X]  Motion sensor  0x68 / 0x69 - no answer");
  if (!found[0x40]) Serial.println("[X]  Motor driver   0x40        - no answer");
  for (uint8_t address = 1; address < 127; ++address) {
    if (!found[address] || address == 0x3C || address == 0x3D || address == 0x68 || address == 0x69 || address == 0x40) continue;
    if (address == 0x70) Serial.println("[i]  0x70 answered: often the driver's all-call address, not another board.");
    else Serial.printf("[?]  0x%02X answered: an address this project does not label.\n", address);
  }
  Serial.println("Names assume our project's parts. An address reply is not a complete device test.");
  Serial.println("[X] is expected for parts set aside in your lesson; check only the parts you connected.");
  Serial.println("Next: type status and press Enter. After wiring changes, restart with output power off.");
}

bool sendPulse(uint8_t channel, int pulse) {
  if (!pcaOK || channel >= 4) return false;
  const Calibration &c = calibration[channel];
  pulse = companion::clampPulse(pulse, c.low, c.high);
  // 50Hz: 20,000us per cycle, 4096 ticks. Oscillator tolerance requires physical calibration.
  uint16_t ticks = static_cast<uint16_t>((pulse * 4096L + 10000) / 20000);
  if (pwm.setPWM(channel, 0, ticks) != 0) {
    stopMotion(); pcaOK = false;
    Serial.println("PCA write failed. Remove servo power, inspect wiring, reboot.");
    return false;
  }
  return true;
}

void showDetails() {
  Serial.printf("timed_test=%d\n", benchTest);
  Serial.printf("OLED=%d MPU=%d PCA=%d storage=%d upright=%d sample=%d demo=%d cal_channel=%d\n",
                oledOK, imuOK, pcaOK, storageOK, upright, imuSampleValid, demo, activeChannel);
  for (int ch = 0; ch < 4; ++ch) {
    const Calibration &c = calibration[ch];
    Serial.printf("%d: low=%d center=%d high=%d direction=%d saved=%d\n",
                  ch, c.low, c.center, c.high, c.direction, c.saved);
  }
}

void showDevice(const char *name, bool ready, uint8_t address, const char *expected) {
  Serial.printf("%s %-14s ", ready ? "[OK]" : "[X] ", name);
  if (address) Serial.printf("0x%02X - ", address);
  else Serial.printf("%s - ", expected);
  Serial.println(ready ? "initialized; ready for its lesson test" : "not ready; may be absent, failed initialization, or lost communication");
}

void showStatus() {
  Serial.println("\n--- Andrew's robot check-in ---");
  Serial.println("[OK] ESP32: running Companion and answering your command.");
  showDevice("OLED screen", oledOK, oledAddress, "0x3C/0x3D");
  showDevice("Motion sensor", imuOK, imuAddress, "0x68/0x69");
  showDevice("Motor driver", pcaOK, PCA_ADDRESS, "0x40");
  Serial.println("These are firmware readiness flags, not a fresh connection scan. Type scan for current replies.");
  Serial.println("A part set aside for this lesson is expected to show [X]. It does not mean you failed!");
  Serial.printf("Timed test: %s | Gesture: %s\n", benchTest ? "RUNNING" : "off", demo ? "RUNNING" : "off");
  if (activeChannel < 0) Serial.println("Calibration channel: none");
  else Serial.printf("Active channel: %d | requested pulse: %d microseconds\n", activeChannel, currentPulse);
  Serial.println("Readiness does not prove drawing, accurate readings, or movement: observe your lesson's result.");
  Serial.println("Type help for commands, details for technical numbers. Press Enter after each command.");
}

void printHelp() {
  Serial.println("Companion bench firmware 0.1.0 | 115200 baud | newline commands");
  Serial.println("help | status | scan | details | stop | face awake/happy/sleep");
  Serial.println("Type a command here, then press Enter. status = friendly report; scan = current address replies.");
  Serial.println("test N      : unloaded servo; center 500ms, +10us 500ms, then pulses off");
  Serial.println("cal N       : arm only channel N (0..3), neutral pulse, unloaded servo only");
  Serial.println("jog N DELTA : move active channel by -10..10 microseconds; 15s timeout");
  Serial.println("center N    : set current pulse as center; requires center inside limits");
  Serial.println("limits N LOW HIGH : stopped only; 1300..1700us absolute envelope");
  Serial.println("invert N    : stopped only; reverse future gesture direction");
  Serial.println("save N      : persist physically checked calibration; stops motion");
  Serial.println("demo        : all 4 saved + upright MPU required; small 10s gesture");
  Serial.println("No command automatically starts walking. stop disables pulses, not servo power.");
}

void loadCalibration() {
  storageOK = preferences.begin("companion", false);
  if (!storageOK) return;
  for (int ch = 0; ch < 4; ++ch) {
    char key[8]; snprintf(key, sizeof(key), "leg%d", ch);
    // Explicit integer fields avoid persisting compiler-dependent structs.
    int values[5] = {};
    if (preferences.getBytesLength(key) != sizeof(values)) continue;
    preferences.getBytes(key, values, sizeof(values));
    if (values[0] == 1 && companion::validLimits(values[1], values[2], values[3]) &&
        (values[4] == 1 || values[4] == -1)) {
      calibration[ch] = {values[1], values[2], values[3], values[4], true};
    }
  }
}

void command(char *line) {
  char *tokens[5] = {};
  int count = 0;
  for (char *t = strtok(line, " \t"); t; t = strtok(nullptr, " \t")) {
    if (count == 5) { Serial.println("Too many arguments."); return; }
    tokens[count++] = t;
  }
  if (!count) return;
  const char *op = tokens[0];
  if (!strcmp(op, "stop") && count == 1) { stopMotion(); Serial.println("Stopped."); return; }
  if (!strcmp(op, "help") && count == 1) { printHelp(); return; }
  if (!strcmp(op, "status") && count == 1) { showStatus(); return; }
  if (!strcmp(op, "details") && count == 1) { showDetails(); return; }
  if (!strcmp(op, "scan") && count == 1) { scanBus(); return; }
  if (benchTest) { Serial.println("Timed test active; use stop or wait for completion."); return; }
  if (!strcmp(op, "face") && count == 2) {
    if (!strcmp(tokens[1], "awake")) mood = Mood::Awake;
    else if (!strcmp(tokens[1], "happy")) mood = Mood::Happy;
    else if (!strcmp(tokens[1], "sleep")) mood = Mood::Sleep;
    else { Serial.println("Use awake, happy or sleep."); return; }
    lastInteraction = millis(); return;
  }
  if (!strcmp(op, "demo") && count == 1) {
    stopMotion();
    for (const Calibration &c : calibration) if (!c.saved) { Serial.println("Save all four physically checked calibrations first."); return; }
    if (!pcaOK || !imuOK || !imuSampleValid || !upright) { Serial.println("Demo needs PCA and upright MPU (+Z up)."); return; }
    demo = true; demoStart = millis(); lastInteraction = demoStart; mood = Mood::Happy;
    Serial.println("10-second small gesture. Keep power disconnect accessible."); return;
  }
  // Reject partial numbers and oversized values instead of silently coercing them.
  long numbers[3] = {};
  if (count < 2 || count > 4) { Serial.println("Unknown command; type help."); return; }
  for (int i = 1; i < count; ++i) {
    char *end = nullptr;
    numbers[i - 1] = strtol(tokens[i], &end, 10);
    if (!*tokens[i] || *end || numbers[i - 1] < -10000 || numbers[i - 1] > 10000) {
      Serial.println("Invalid integer."); return;
    }
  }
  int ch = numbers[0];
  if (ch < 0 || ch > 3) { Serial.println("Channel must be 0..3."); return; }
  Calibration &c = calibration[ch];
  if (!strcmp(op, "test") && count == 2) {
    if (activeChannel >= 0 || demo) { Serial.println("Use stop before test."); return; }
    if (!pcaOK) { Serial.println("PCA unavailable."); return; }
    if (c.center + 10 > c.high) { Serial.println("Test needs 10us above center inside existing limits; do not widen blindly."); return; }
    stopMotion();
    currentPulse = c.center;
    if (sendPulse(ch, currentPulse)) {
      activeChannel = ch; benchStart = millis(); lastCommand = benchStart;
      benchOffsetSent = false; benchTest = true;
      Serial.printf("Test channel %d: %dus for 500ms, then %dus for 500ms. Observe unloaded shaft.\n", ch, c.center, c.center + 10);
    }
  } else if (!strcmp(op, "cal") && count == 2) {
    stopMotion();
    if (!pcaOK) { Serial.println("PCA unavailable."); return; }
    currentPulse = c.center;
    if (sendPulse(ch, currentPulse)) {
      activeChannel = ch; lastCommand = millis();
      Serial.printf("Channel %d active at %dus. Unloaded servo only.\n", ch, currentPulse);
    }
  } else if (!strcmp(op, "jog") && count == 3 && activeChannel == ch && !demo) {
    int delta = numbers[1];
    if (delta < -10 || delta > 10) { Serial.println("Jog limited to +/-10us per command."); return; }
    currentPulse = companion::clampPulse(currentPulse + delta, c.low, c.high);
    sendPulse(ch, currentPulse); lastCommand = millis();
    Serial.printf("%dus\n", currentPulse);
  } else if (!strcmp(op, "center") && count == 2 && activeChannel == ch && !demo) {
    if (!companion::validLimits(c.low, currentPulse, c.high)) { Serial.println("Center must be strictly inside limits."); return; }
    c.center = currentPulse; c.saved = false; lastCommand = millis();
  } else if (!strcmp(op, "limits") && count == 4 && activeChannel < 0 && !demo) {
    if (!companion::validLimits(numbers[1], c.center, numbers[2])) { Serial.println("Invalid limits; keep center inside 1300..1700us."); return; }
    c.low = numbers[1]; c.high = numbers[2]; c.saved = false;
  } else if (!strcmp(op, "invert") && count == 2 && activeChannel < 0 && !demo) {
    c.direction *= -1; c.saved = false;
  } else if (!strcmp(op, "save") && count == 2) {
    stopMotion();
    if (!storageOK) { Serial.println("Storage unavailable; not saved."); return; }
    char key[8]; snprintf(key, sizeof(key), "leg%d", ch);
    int values[5] = {1, c.low, c.center, c.high, c.direction};
    c.saved = preferences.putBytes(key, values, sizeof(values)) == sizeof(values);
    Serial.println(c.saved ? "Saved. This records your approval, not an automatic physical test." : "Save failed.");
  } else {
    Serial.println("Invalid command/state; use cal before jog, stop before limits/invert.");
  }
}

void pollSerial() {
  // Bound work per loop so continuous input cannot starve sensing or stop timers.
  for (int budget = 0; budget < 64 && Serial.available(); ++budget) {
    char c = Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      if (!overflowed) { input[used] = 0; command(input); }
      else Serial.println("Line too long; discarded and motion stopped.");
      used = 0; overflowed = false;
    } else if (!overflowed) {
      if (used < sizeof(input) - 1) input[used++] = c;
      else { overflowed = true; stopMotion(); }
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(6, 7); Wire.setClock(100000); Wire.setTimeOut(25);
  // Initialize PCA before display and sensor. This cannot prevent pulses retained
  // through a controller-only reset before setup runs; see power-on instructions.
  pcaOK = probe(PCA_ADDRESS) && pwm.begin();
  if (pcaOK) { stopMotion(); pwm.setPWMFreq(50); stopMotion(); }
  loadCalibration();
  oledAddress = probe(0x3C) ? 0x3C : (probe(0x3D) ? 0x3D : 0);
  if (oledAddress) oledOK = screen.begin(SSD1306_SWITCHCAPVCC, oledAddress, true, false);
  imuAddress = probe(0x68) ? 0x68 : (probe(0x69) ? 0x69 : 0);
  if (imuAddress) imuOK = imu.begin(imuAddress, &Wire);
  if (imuOK) {
    imu.setAccelerometerRange(MPU6050_RANGE_8_G);
    imu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  }
  Wire.setClock(100000);
  lastInteraction = millis();
  printHelp(); showStatus();
}

void loop() {
  pollSerial();
  uint32_t now = millis();
  if (imuOK && companion::elapsed(now, lastSensor) >= 25) {
    lastSensor = now;
    sensors_event_t a, g, t;
    imuSampleValid = imu.getEvent(&a, &g, &t);
    if (!imuSampleValid) { stopMotion(); imuOK = false; mood = Mood::Concerned; }
    else {
      ax = a.acceleration.x; ay = a.acceleration.y; az = a.acceleration.z;
      imuSampleValid = isfinite(ax) && isfinite(ay) && isfinite(az);
      if (!imuSampleValid) { stopMotion(); imuOK = false; }
      else {
        upright = companion::uprightState(upright, az);
        if (demo && !upright) { stopMotion(); mood = Mood::Concerned; Serial.println("Tilt stop. Re-arm manually when upright."); }
        if (!demo && activeChannel < 0 && taps.update(sqrtf(ax*ax + ay*ay + az*az), now)) {
          mood = Mood::Happy; lastInteraction = now; Serial.println("Double tap! Face awake; motion remains off.");
        }
      }
    }
  }
  if (companion::elapsed(now, lastHealth) >= 500) {
    lastHealth = now;
    if ((pcaOK && !probe(PCA_ADDRESS)) || (imuOK && !probe(0x68) && !probe(0x69))) {
      stopMotion(); pcaOK = false; imuOK = false; imuSampleValid = false;
      Serial.println("I2C health check failed. Remove servo power and reboot after repair.");
    }
  }
  if (benchTest) {
    const int phase = companion::benchPhase(now, benchStart);
    if (phase == 2) {
      stopMotion(); Serial.println("Timed test ended; pulse-off requested. Confirm movement yourself; remove supply power.");
    } else if (phase == 1 && !benchOffsetSent) {
      benchOffsetSent = true;
      currentPulse = calibration[activeChannel].center + 10;
      sendPulse(activeChannel, currentPulse);
    }
  }
  if (activeChannel >= 0 && companion::elapsed(now, lastCommand) >= CAL_TIMEOUT) {
    stopMotion(); Serial.println("Calibration timed out; pulses disabled.");
  }
  if (demo) {
    if (companion::elapsed(now, demoStart) >= 10000) { stopMotion(); Serial.println("Gesture complete; pulses disabled."); }
    else if (companion::elapsed(now, lastMotion) >= 20) {
      lastMotion = now;
      float phase = companion::elapsed(now, demoStart) / 1000.0f * 2 * PI / 3;
      for (int ch = 0; ch < 4 && demo; ++ch) {
        const Calibration &c = calibration[ch];
        int amplitude = min(25, min(c.center - c.low, c.high - c.center));
        int pulse = c.center + c.direction * static_cast<int>(amplitude * sinf(phase));
        sendPulse(ch, pulse);
      }
    }
  }
  if (!demo && activeChannel < 0 && companion::elapsed(now, lastInteraction) >= IDLE_TIMEOUT) mood = Mood::Sleep;
  if (oledOK && companion::elapsed(now, lastFace) >= 120) {
    lastFace = now; drawFace(screen, mood, now, demo || activeChannel >= 0, imuOK);
  }
  if (companion::elapsed(now, lastTelemetry) >= 2000) {
    lastTelemetry = now;
    if (imuOK) Serial.printf("accel m/s2: %.2f %.2f %.2f | upright=%d\n", ax, ay, az, upright);
  }
}
