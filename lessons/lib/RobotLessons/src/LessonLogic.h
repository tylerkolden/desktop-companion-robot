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
// These are software ceilings, NOT guaranteed safe physical endpoints.
constexpr bool validRange(int low, int high) {
  return low>=500 && low<=1450 && high>=1550 && high<=2500;
}
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
constexpr int approach(int current, int target) {
  return current<target ? (target-current>5 ? current+5 : target)
       : current>target ? (current-target>5 ? current-5 : target) : current;
}
constexpr bool sweepEnvelope() {
  for(uint32_t t=0;t<=TEST_MS;t+=20)
    if(sweepPulse(t,500,2500)<500 || sweepPulse(t,500,2500)>2500) return false;
  return true;
}
static_assert(sweepEnvelope(), "sweep stays inside selected endpoints");
static_assert(sweepPulse(2000,800,2200)==800 && sweepPulse(5500,800,2200)==2200, "both endpoints reached");
static_assert(sweepPulse(8000,800,2200)==1500 && sweepPulse(56000,800,2200)==1500, "cycle and final center");
static_assert(sweepPulse(1000,1000,2100)==1250 && sweepPulse(4000,1000,2100)==1550, "asymmetric range ramps");
static_assert(!validRange(499,2500) && !validRange(500,2501) && !validRange(1600,1400), "reject invalid limits");
static_assert(approach(1500,1525)==1505 && approach(1500,1475)==1495 && approach(1503,1500)==1500, "calibration steps are ramped");
static_assert(finished(59744,0xffffff00u,TEST_MS), "minute deadline across millis rollover");
static_assert(!finished(59743,0xffffff00u,TEST_MS), "minute deadline boundary");
static_assert(finished(100,0,SAMPLE_MS) && !finished(99,0,SAMPLE_MS), "100ms sample boundary");
static_assert(degreesPerSecond(1)>57.29f && degreesPerSecond(1)<57.30f, "gyro units");
}
