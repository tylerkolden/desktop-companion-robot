#pragma once
#include <stdint.h>

namespace companion {
// Eight seconds: center, three low/high pairs, return to center, then off.
constexpr int wiggleTarget(int phase) {
  return phase <= 0 || phase >= 7 ? 0 : (phase % 2 ? -50 : 50);
}
constexpr int wiggleOffset(uint32_t age) {
  if (age >= 8000) return 0;
  const int phase = age / 1000;
  const int progress = age % 1000 < 500 ? age % 1000 : 500;
  const int from = wiggleTarget(phase - 1), to = wiggleTarget(phase);
  return from + (to - from) * progress / 500;
}
constexpr int wiggleAmplitude(int low, int center, int high) {
  const int room = center-low < high-center ? center-low : high-center;
  return room < 50 ? room : 50;
}
static_assert(wiggleOffset(0) == 0, "start at center");
static_assert(wiggleOffset(1250) == -25, "ramp rather than jump");
static_assert(wiggleOffset(1500) == -50, "low endpoint");
static_assert(wiggleOffset(2500) == 50, "high endpoint");
static_assert(wiggleOffset(7500) == 0, "return to center");
static_assert(wiggleOffset(8000) == 0 && wiggleOffset(20000) == 0, "expired trajectory");
static_assert(wiggleAmplitude(1490,1500,1550) == 10, "respect asymmetric limits");
static_assert(wiggleAmplitude(1300,1500,1700) == 50, "never expand test amplitude");
constexpr bool wiggleEnvelope() {
  for (uint32_t t=0; t<9000; ++t)
    if (wiggleOffset(t)<-50 || wiggleOffset(t)>50) return false;
  return true;
}
static_assert(wiggleEnvelope(), "all trajectory samples stay in bounds");
}
