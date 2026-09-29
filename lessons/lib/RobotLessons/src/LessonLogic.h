#pragma once
#include <stdint.h>
namespace lesson {
constexpr uint32_t TEST_MS=60000;
constexpr uint32_t SAMPLE_MS=100;
constexpr uint32_t elapsed(uint32_t now, uint32_t start) { return now-start; }
constexpr bool finished(uint32_t now, uint32_t start, uint32_t duration) {
  return elapsed(now,start) >= duration;
}
constexpr float degreesPerSecond(float radians) { return radians*57.295779513f; }
// Moderate lesson preset, not measured mechanical endpoints or a claimed angle.
constexpr int PRACTICE_LOW=1300;
constexpr int PRACTICE_HIGH=1700;
constexpr int ramp(int from, int to, uint32_t age, uint32_t duration) {
  return from+(to-from)*static_cast<int32_t>(age)/static_cast<int32_t>(duration);
}
constexpr int sweepPulse(uint32_t age, int low, int high) {
  // Center -> low -> high -> center, seven times, then four seconds centered.
  if(age>=56000) return 1500;
  age%=8000;
  if(age<2000) return ramp(1500,low,age,2000);
  if(age<2500) return low;
  if(age<5500) return ramp(low,high,age-2500,3000);
  if(age<6000) return high;
  return ramp(high,1500,age-6000,2000);
}
constexpr int practicePulse(uint32_t age) {
  return sweepPulse(age,PRACTICE_LOW,PRACTICE_HIGH);
}
constexpr bool sweepEnvelope() {
  for(uint32_t t=0;t<=TEST_MS;t+=20)
    if(practicePulse(t)<PRACTICE_LOW || practicePulse(t)>PRACTICE_HIGH) return false;
  return true;
}
static_assert(practicePulse(2000)==1300 && practicePulse(5500)==1700 && practicePulse(TEST_MS)==1500, "preset endpoints and finish");
static_assert(sweepEnvelope(), "sweep stays inside selected endpoints");
static_assert(sweepPulse(2000,800,2200)==800 && sweepPulse(5500,800,2200)==2200, "both endpoints reached");
static_assert(sweepPulse(8000,800,2200)==1500 && sweepPulse(56000,800,2200)==1500, "cycle and final center");
static_assert(sweepPulse(1000,1000,2100)==1250 && sweepPulse(4000,1000,2100)==1550, "asymmetric range ramps");
static_assert(finished(59744,0xffffff00u,TEST_MS), "minute deadline across millis rollover");
static_assert(!finished(59743,0xffffff00u,TEST_MS), "minute deadline boundary");
static_assert(finished(100,0,SAMPLE_MS) && !finished(99,0,SAMPLE_MS), "100ms sample boundary");
static_assert(degreesPerSecond(1)>57.29f && degreesPerSecond(1)<57.30f, "gyro units");
}
