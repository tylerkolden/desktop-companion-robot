#pragma once
#include <stdint.h>
namespace lesson {
constexpr uint32_t elapsed(uint32_t now, uint32_t start) { return now-start; }
constexpr bool finished(uint32_t now, uint32_t start, uint32_t duration) {
  return elapsed(now,start) >= duration;
}
constexpr int target(int phase) { return phase<=0 || phase>=7 ? 0 : (phase%2 ? -50 : 50); }
constexpr int motorPulse(uint32_t age) {
  if (age>=8000) return 1500;
  const int phase=age/1000;
  const int fraction=age%1000<500 ? age%1000 : 500;
  return 1500+target(phase-1)+(target(phase)-target(phase-1))*fraction/500;
}
constexpr float degreesPerSecond(float radians) { return radians*57.295779513f; }
constexpr bool envelope() {
  for (uint32_t t=0;t<9000;t++) if (motorPulse(t)<1450 || motorPulse(t)>1550) return false;
  return true;
}
static_assert(envelope(), "servo trajectory must stay within tested bench envelope");
static_assert(motorPulse(1250)==1475 && motorPulse(2500)==1550, "ramp and endpoint");
static_assert(motorPulse(7500)==1500 && motorPulse(9000)==1500, "return and late tick");
static_assert(finished(7744,0xffffff00u,8000), "deadline across millis rollover");
static_assert(!finished(7743,0xffffff00u,8000), "deadline boundary");
static_assert(degreesPerSecond(1)>57.29f && degreesPerSecond(1)<57.30f, "gyro units");
}
