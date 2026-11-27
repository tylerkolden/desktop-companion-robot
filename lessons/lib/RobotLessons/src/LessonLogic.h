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
// Wider lesson preset, not measured mechanical endpoints or a claimed angle.
constexpr int PRACTICE_LOW=500;
constexpr int PRACTICE_HIGH=2500;
static_assert((1500-PRACTICE_LOW)==500*2 && (PRACTICE_HIGH-1500)==500*2, "2x previous pulse excursion each side");
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
static_assert(practicePulse(2000)==500 && practicePulse(5500)==2500 && practicePulse(TEST_MS)==1500, "preset endpoints and finish");
static_assert(sweepEnvelope(), "sweep stays inside selected endpoints");
static_assert(sweepPulse(2000,800,2200)==800 && sweepPulse(5500,800,2200)==2200, "both endpoints reached");
static_assert(sweepPulse(8000,800,2200)==1500 && sweepPulse(56000,800,2200)==1500, "cycle and final center");
static_assert(sweepPulse(1000,1000,2100)==1250 && sweepPulse(4000,1000,2100)==1550, "asymmetric range ramps");
// Leg lesson: gentler fixed poses with long holds so a learner can measure angles.
// Narrower than the practice sweep. Pulse requests only; angles must be measured.
constexpr int LEG_POSE_COUNT=9;
constexpr int LEG_POSES[LEG_POSE_COUNT]={1500,1250,1500,1750,1500,1000,1500,2000,1500};
constexpr uint32_t LEG_STEP_MS=6000, LEG_RAMP_MS=1500;
constexpr int LEG_LOW=1000, LEG_HIGH=2000;
constexpr int legPose(uint32_t age) {
  return age>=LEG_POSE_COUNT*LEG_STEP_MS ? -1 : static_cast<int>(age/LEG_STEP_MS);
}
constexpr bool legHolding(uint32_t age) {
  return legPose(age)>=0 && age%LEG_STEP_MS>=LEG_RAMP_MS;
}
constexpr int legPulse(uint32_t age) {
  const int pose=legPose(age);
  if(pose<0) return 1500;
  const int from=pose==0 ? 1500 : LEG_POSES[pose-1];
  const uint32_t into=age%LEG_STEP_MS;
  return into<LEG_RAMP_MS ? ramp(from,LEG_POSES[pose],into,LEG_RAMP_MS) : LEG_POSES[pose];
}
constexpr bool legEnvelope() {
  for(uint32_t t=0;t<=TEST_MS;t+=20)
    if(legPulse(t)<LEG_LOW || legPulse(t)>LEG_HIGH) return false;
  return true;
}
static_assert(legEnvelope(), "leg poses stay inside 1000-2000us");
static_assert(legPulse(0)==1500 && legPulse(TEST_MS)==1500, "leg starts and ends centered");
static_assert(legPulse(LEG_STEP_MS+LEG_RAMP_MS)==1250 && legPulse(3*LEG_STEP_MS+LEG_RAMP_MS)==1750, "first two measuring poses");
static_assert(legPulse(LEG_STEP_MS+750)==1375, "leg ramps gently between poses");
static_assert(legHolding(LEG_STEP_MS+LEG_RAMP_MS) && !legHolding(LEG_STEP_MS+LEG_RAMP_MS-1) && !legHolding(TEST_MS), "hold windows");
static_assert(LEG_POSE_COUNT*LEG_STEP_MS<=TEST_MS, "poses fit inside the minute");
static_assert(finished(59744,0xffffff00u,TEST_MS), "minute deadline across millis rollover");
static_assert(!finished(59743,0xffffff00u,TEST_MS), "minute deadline boundary");
static_assert(finished(100,0,SAMPLE_MS) && !finished(99,0,SAMPLE_MS), "100ms sample boundary");
static_assert(degreesPerSecond(1)>57.29f && degreesPerSecond(1)<57.30f, "gyro units");
}
