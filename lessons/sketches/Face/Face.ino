#include <Wire.h>
#include <Adafruit_SSD1306.h>
// XIAO + OLED only. Timer-driven eyes; no servo commands.
Adafruit_SSD1306 screen(128, 64, &Wire, -1);
bool ready = false, sleepy = false;
unsigned long lastChange = 0;
void setup() {
  Serial.begin(115200);
  Wire.begin(6, 7); Wire.setTimeOut(25);
  uint8_t address = 0;
  for (uint8_t candidate = 0x3C; candidate <= 0x3D; ++candidate) {
    Wire.beginTransmission(candidate);
    if (Wire.endTransmission() == 0) { address = candidate; break; }
  }
  ready = address && screen.begin(SSD1306_SWITCHCAPVCC, address, true, false);
  if (ready) { screen.clearDisplay(); screen.display(); }
}
void loop() {
  unsigned long now = millis();
  if (now - lastChange < 1000) return;
  lastChange = now;
  if (!ready) { Serial.println("OLED not ready. Power off, check wiring, try Scanner."); return; }
  sleepy = !sleepy;
  screen.clearDisplay();
  int height = sleepy ? 4 : 24;
  screen.fillRoundRect(25, 22, 24, height, 2, SSD1306_WHITE);
  screen.fillRoundRect(79, 22, 24, height, 2, SSD1306_WHITE);
  screen.display();
  Serial.println(sleepy ? "sleepy eyes" : "awake eyes");
}
