#pragma once
#include <stdint.h>
namespace lesson {
constexpr uint32_t TEST_MS=60000;
constexpr uint32_t SAMPLE_MS=100;
constexpr uint32_t elapsed(uint32_t now, uint32_t start) { return now-start; }
constexpr bool finished(uint32_t now, uint32_t start, uint32_t duration) {
  return elapsed(now,start) >= duration;
}
constexpr int target(int phase) { return phase<=0 || phase>=7 ? 0 : (phase%2 ? -50 : 50); }
constexpr int motorPulse(uint32_t age) {
  // Seven bounded eight-second cycles, then rest at center for four seconds.
  if (age>=56000) return 1500;
  age%=8000;
  const int phase=age/1000;
  const int fraction=age%1000<500 ? age%1000 : 500;
  return 1500+target(phase-1)+(target(phase)-target(phase-1))*fraction/500;
}
constexpr float degreesPerSecond(float radians) { return radians*57.295779513f; }
constexpr bool envelope() {
  for (uint32_t t=0;t<=TEST_MS;t+=20) if (motorPulse(t)<1450 || motorPulse(t)>1550) return false;
  return true;
}
static_assert(envelope(), "servo trajectory must stay within tested bench envelope");
static_assert(motorPulse(1250)==1475 && motorPulse(2500)==1550, "ramp and endpoint");
static_assert(motorPulse(7500)==1500 && motorPulse(9500)==1450, "return and next cycle");
static_assert(motorPulse(56000)==1500 && motorPulse(TEST_MS)==1500, "final center and expiry");
static_assert(finished(59744,0xffffff00u,TEST_MS), "minute deadline across millis rollover");
static_assert(!finished(59743,0xffffff00u,TEST_MS), "minute deadline boundary");
static_assert(finished(100,0,SAMPLE_MS) && !finished(99,0,SAMPLE_MS), "100ms sample boundary");
static_assert(degreesPerSecond(1)>57.29f && degreesPerSecond(1)<57.30f, "gyro units");
}
