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
// Dog lesson: four legs. Angles are in degrees, + = foot forward. The sketch turns them into
// pulses with each leg's own center and direction. Requests only; nothing here is measured.
constexpr int DOG_LEGS=4;
constexpr int DOG_MAX_DEG=40;
// Part 1 (0-18s): stand, then each leg alone swings forward 20 degrees so the learner can
// check its direction and channel. Part 2 (18-60s): named poses.
constexpr uint32_t DOG_CHECK_START=2000, DOG_CHECK_STEP=4000, DOG_POSE_START=18000;
constexpr uint32_t DOG_POSE_MS=7000, DOG_POSE_RAMP=1500;
constexpr int DOG_POSE_COUNT=6;
constexpr int DOG_STAND=0, DOG_SIT=1, DOG_BOW=2;
// Pose angles per leg: front-left, front-right, back-left, back-right.
constexpr int DOG_POSE_DEG[3][DOG_LEGS]={{0,0,0,0},{0,0,35,35},{-35,-35,0,0}};
constexpr int DOG_SEQUENCE[DOG_POSE_COUNT]={DOG_SIT,DOG_STAND,DOG_BOW,DOG_STAND,DOG_SIT,DOG_STAND};
constexpr int dogCheckLeg(uint32_t age) {
  return age<DOG_CHECK_START || (age-DOG_CHECK_START)/DOG_CHECK_STEP>=DOG_LEGS ? -1
    : static_cast<int>((age-DOG_CHECK_START)/DOG_CHECK_STEP);
}
constexpr int dogCheckDeg(uint32_t into) {   // 1s out, 2s hold, 1s back
  return into<1000 ? ramp(0,20,into,1000) : into<3000 ? 20 : into<4000 ? ramp(20,0,into-3000,1000) : 0;
}
constexpr int dogPoseIndex(uint32_t age) {
  return age<DOG_POSE_START || age>=DOG_POSE_START+DOG_POSE_COUNT*DOG_POSE_MS ? -1
    : static_cast<int>((age-DOG_POSE_START)/DOG_POSE_MS);
}
constexpr bool dogHolding(uint32_t age) {
  return dogPoseIndex(age)>=0 && (age-DOG_POSE_START)%DOG_POSE_MS>=DOG_POSE_RAMP;
}
constexpr bool dogCheckHolding(uint32_t age) {
  return dogCheckLeg(age)>=0 && (age-DOG_CHECK_START)%DOG_CHECK_STEP>=1000 && (age-DOG_CHECK_START)%DOG_CHECK_STEP<3000;
}
constexpr int dogAngle(int leg, uint32_t age) {
  if(age<DOG_POSE_START) {
    const int checking=dogCheckLeg(age);
    return checking==leg ? dogCheckDeg((age-DOG_CHECK_START)%DOG_CHECK_STEP) : 0;
  }
  const int pose=dogPoseIndex(age);
  if(pose<0) return 0;
  const int from=pose==0 ? DOG_POSE_DEG[DOG_STAND][leg] : DOG_POSE_DEG[DOG_SEQUENCE[pose-1]][leg];
  const int to=DOG_POSE_DEG[DOG_SEQUENCE[pose]][leg];
  const uint32_t into=(age-DOG_POSE_START)%DOG_POSE_MS;
  return into<DOG_POSE_RAMP ? ramp(from,to,into,DOG_POSE_RAMP) : to;
}
constexpr bool dogEnvelope() {
  for(int leg=0;leg<DOG_LEGS;leg++)
    for(uint32_t t=0;t<=TEST_MS;t+=20) { const int a=dogAngle(leg,t); if(a<-DOG_MAX_DEG || a>DOG_MAX_DEG) return false; }
  return true;
}
static_assert(dogEnvelope(), "dog angles stay inside +/-40 degrees");
static_assert(DOG_POSE_START+DOG_POSE_COUNT*DOG_POSE_MS<=TEST_MS, "dog routine fits the minute");
static_assert(DOG_CHECK_START+DOG_LEGS*DOG_CHECK_STEP<=DOG_POSE_START, "leg check finishes before poses");
static_assert(dogAngle(0,0)==0 && dogAngle(3,TEST_MS)==0, "dog starts and ends standing");
static_assert(dogCheckLeg(DOG_CHECK_START+2000)==0 && dogCheckLeg(DOG_CHECK_START+3*DOG_CHECK_STEP+2000)==3, "legs checked in order");
static_assert(dogAngle(0,DOG_CHECK_START+2000)==20 && dogAngle(1,DOG_CHECK_START+2000)==0, "one leg at a time");
static_assert(dogAngle(2,DOG_POSE_START+DOG_POSE_RAMP)==35 && dogAngle(0,DOG_POSE_START+DOG_POSE_RAMP)==0, "first pose is sit");
static_assert(dogAngle(2,DOG_POSE_START+750)==17, "dog ramps between poses");
static_assert(dogAngle(DOG_LEGS-1,DOG_POSE_START+DOG_POSE_COUNT*DOG_POSE_MS-1)==0, "dog finishes standing");

// Tail servo (Puppy V2 + tail, 2026-11-28): PCA channel 4, after the four legs on 0-3.
// The tail stays centred during the leg check and while sitting or bowing. During each STAND
// hold it wags 5 times (+/-25 degrees, 0.8s per wag), then rests centred before the next pose.
// Same-side legs touch if the front paw swings far back while the back paw swings far forward
// (Puppy V2 CAD, checked every 5 deg: first touch at front -55 with back +65). Keep 10 deg spare.
constexpr int DOG_FRONT_BACK_LIMIT=-45, DOG_REAR_FORWARD_LIMIT=45;
constexpr bool dogSideClear(int front,int rear) { return front>=DOG_FRONT_BACK_LIMIT || rear<=DOG_REAR_FORWARD_LIMIT; }
static_assert(DOG_MAX_DEG<=DOG_REAR_FORWARD_LIMIT && -DOG_MAX_DEG>=DOG_FRONT_BACK_LIMIT, "today's poses can never make the legs touch");
static_assert(dogSideClear(90,90) && dogSideClear(-60,0) && dogSideClear(0,70) && !dogSideClear(-60,60), "lie flat, play bow and sit are clear; front back + rear forward is not");

constexpr int DOG_TAIL_CHANNEL=DOG_LEGS;
constexpr int DOG_TAIL_MAX_DEG=30, DOG_TAIL_WAG_DEG=25;
constexpr uint32_t DOG_WAG_MS=800, DOG_WAG_COUNT=5;
constexpr int dogWagDeg(uint32_t into) {   // 0 -> +25 -> 0 -> -25 -> 0, 200ms each part
  return into<200 ? ramp(0,DOG_TAIL_WAG_DEG,into,200)
    : into<400 ? ramp(DOG_TAIL_WAG_DEG,0,into-200,200)
    : into<600 ? ramp(0,-DOG_TAIL_WAG_DEG,into-400,200)
    : ramp(-DOG_TAIL_WAG_DEG,0,into-600,200);
}
constexpr int dogTailAngle(uint32_t age) {
  const int pose=dogPoseIndex(age);
  if(pose<0 || DOG_SEQUENCE[pose]!=DOG_STAND || !dogHolding(age)) return 0;
  const uint32_t into=(age-DOG_POSE_START)%DOG_POSE_MS-DOG_POSE_RAMP;
  return into<DOG_WAG_MS*DOG_WAG_COUNT ? dogWagDeg(into%DOG_WAG_MS) : 0;
}
constexpr bool dogTailEnvelope() {
  for(uint32_t t=0;t<=TEST_MS;t+=10) { const int a=dogTailAngle(t); if(a<-DOG_TAIL_MAX_DEG || a>DOG_TAIL_MAX_DEG) return false; }
  for(uint32_t t=10;t<=TEST_MS;t+=10) { const int d=dogTailAngle(t)-dogTailAngle(t-10); if(d>3 || d<-3) return false; }   // no jumps
  return true;
}
static_assert(dogTailEnvelope(), "tail stays inside +/-30 degrees and never jumps");
static_assert(DOG_POSE_RAMP+DOG_WAG_MS*DOG_WAG_COUNT<DOG_POSE_MS, "wagging finishes before the next pose");
static_assert(dogTailAngle(0)==0 && dogTailAngle(DOG_POSE_START-1)==0 && dogTailAngle(TEST_MS)==0, "tail still during the leg check and at the end");
static_assert(dogTailAngle(DOG_POSE_START+DOG_POSE_RAMP+100)==0, "tail still while sitting (first pose)");
static_assert(dogTailAngle(DOG_POSE_START+DOG_POSE_MS+DOG_POSE_RAMP+200)==DOG_TAIL_WAG_DEG
  && dogTailAngle(DOG_POSE_START+DOG_POSE_MS+DOG_POSE_RAMP+600)==-DOG_TAIL_WAG_DEG, "tail wags while standing");
static_assert(finished(59744,0xffffff00u,TEST_MS), "minute deadline across millis rollover");
static_assert(!finished(59743,0xffffff00u,TEST_MS), "minute deadline boundary");
static_assert(finished(100,0,SAMPLE_MS) && !finished(99,0,SAMPLE_MS), "100ms sample boundary");
static_assert(degreesPerSecond(1)>57.29f && degreesPerSecond(1)<57.30f, "gyro units");
}
