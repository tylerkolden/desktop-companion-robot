#pragma once
#include <Adafruit_SSD1306.h>
#include <math.h>

enum class Mood { Awake, Happy, Sleep, Concerned };
inline void drawFace(Adafruit_SSD1306 &screen, Mood mood, uint32_t now,
                     bool armed, bool imuPresent) {
  screen.clearDisplay();
  int drift = static_cast<int>(3 * sinf(now / 1800.0f));
  bool blink = now % 4300 < 130;
  int height = (blink || mood == Mood::Sleep) ? 3 : 26;
  int top = 20 + (26 - height) / 2;
  for (int x : {22, 76}) {
    screen.fillRoundRect(x + drift, top, 30, height, height > 4 ? 8 : 1, SSD1306_WHITE);
    if (mood == Mood::Happy && !blink) {
      screen.fillRoundRect(x + drift - 1, top + 12, 32, 23, 9, SSD1306_BLACK);
    }
    if (mood == Mood::Concerned && !blink) {
      screen.fillTriangle(x + drift, top, x + drift + 30, top,
                          x + drift + 30, top + 9, SSD1306_BLACK);
    }
  }
  screen.setTextSize(1);
  screen.setTextColor(SSD1306_WHITE);
  screen.setCursor(2, 55);
  screen.print(armed ? "SERVO ACTIVE" : "SERVO RESTING");
  if (!imuPresent) { screen.setCursor(105, 2); screen.print("IMU?"); }
  if (mood == Mood::Sleep) { screen.setCursor(108, 10); screen.print("z"); }
  screen.display();
}
