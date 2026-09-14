#pragma once
#include <stdint.h>

namespace companion {
constexpr uint32_t elapsed(uint32_t now, uint32_t then) { return now - then; }
constexpr int clampPulse(int value, int low, int high) {
  return value < low ? low : (value > high ? high : value);
}
constexpr bool validLimits(int low, int center, int high) {
  return low >= 1300 && high <= 1700 && low < center && center < high;
}
// Magnitude thresholds are provisional and must be tuned on the actual rigid mount.
struct TapDetector {
  bool pending = false;
  bool high = false;
  uint32_t first = 0;
  constexpr bool update(float magnitude, uint32_t now) {
    if (pending && elapsed(now, first) > 500) pending = false;
    const bool rising = !high && magnitude > 15.0f;
    if (magnitude < 12.0f) high = false;
    if (!rising) return false;
    high = true;
    if (pending) {
      const uint32_t gap = elapsed(now, first);
      if (gap >= 100 && gap <= 500) { pending = false; return true; }
      return false;
    }
    pending = true;
    first = now;
    return false;
  }
};
constexpr bool uprightState(bool previous, float z) {
  return previous ? z > 3.0f : z > 7.0f;
}
}
