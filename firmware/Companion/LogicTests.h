#pragma once
#include "Logic.h"
// Build-time regression tests execute the same pure logic used by the firmware.
namespace companion_tests {
constexpr bool tapSequence() {
  companion::TapDetector t;
  if (t.update(16, 1000)) return false;
  if (t.update(18, 1100)) return false; // held high is not a second tap
  t.update(9.8f, 1150);
  return t.update(16, 1300);
}
constexpr bool expiredTap() {
  companion::TapDetector t;
  t.update(16, 1000); t.update(9.8f, 1200);
  return !t.update(16, 1600);
}
constexpr bool bounce() {
  companion::TapDetector t;
  t.update(16, 1000); t.update(9, 1010);
  return !t.update(16, 1050);
}
constexpr bool wrappedTap() {
  companion::TapDetector t;
  t.update(16, 0xFFFFFF00u); t.update(9, 0xFFFFFF20u);
  return t.update(16, 44);
}
static_assert(companion::benchPhase(499, 0) == 0, "bench center duration");
static_assert(companion::benchPhase(500, 0) == 1, "bench offset boundary");
static_assert(companion::benchPhase(999, 0) == 1, "bench offset duration");
static_assert(companion::benchPhase(1000, 0) == 2, "bench automatic stop");
static_assert(companion::benchPhase(2000, 0) == 2, "late loop must stop");
static_assert(companion::benchPhase(744, 0xFFFFFF00u) == 2, "bench rollover stop");
static_assert(tapSequence(), "double tap or edge detection regression");
static_assert(expiredTap(), "expired tap accepted");
static_assert(bounce(), "tap bounce accepted");
static_assert(wrappedTap(), "millis rollover breaks taps");
static_assert(companion::elapsed(20, 0xFFFFFFF0u) == 36, "rollover timer");
static_assert(companion::clampPulse(1200, 1400, 1600) == 1400, "low clamp");
static_assert(companion::clampPulse(1800, 1400, 1600) == 1600, "high clamp");
static_assert(companion::clampPulse(1500, 1400, 1600) == 1500, "center clamp");
static_assert(!companion::validLimits(1600, 1500, 1400), "reversed limits");
static_assert(!companion::validLimits(500, 1500, 2500), "unsafe limits");
static_assert(companion::validLimits(1450, 1500, 1550), "valid limits");
static_assert(companion::uprightState(true, 5), "orientation hysteresis");
static_assert(!companion::uprightState(false, 5), "orientation hysteresis");
}
