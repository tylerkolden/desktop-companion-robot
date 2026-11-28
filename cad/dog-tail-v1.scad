// Dog tail V1 (Puppy V2 + tail servo): a wagging tail that screws onto the tail servo exactly
// like a leg. It is Leg V6 turned upside down (the long part points UP), with the same round
// "thigh" disc as Leg V8 to hide the servo, and a little curl at the tip.
// The hip end (horn pocket, screw holes) is IDENTICAL to Leg V6 (owner: perfect fit, 2026-11-27).
// The servo turns it around the shaft, so it wags side to side.
// Print pocket side UP, no supports. Fitting order is the same as a leg.
use <leg-v6.scad>
use <leg-v8.scad>
$fn=64;
leg_t=8.5; leg_len=45;
curl=[[0,leg_len],[3,leg_len+5],[8,leg_len+7.5],[12.5,leg_len+6]];
module tail() {
  mirror([0,1,0]) leg();      // long part now points up (+y)
  thigh();
  for(i=[0:len(curl)-2]) linear_extrude(leg_t) hull() { translate(curl[i]) circle(d=10-i*1.5); translate(curl[i+1]) circle(d=10-(i+1)*1.5); }
}
tail();
