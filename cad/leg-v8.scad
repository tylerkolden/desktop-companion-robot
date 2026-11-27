// Leg V8 (Puppy V2): Leg V7 (round paw) plus a big round THIGH around the hip.
// The thigh hides the servo and the U-slot in the body shell, and is what the dog rests on
// when it lies down. The hip end (horn pocket, screw holes) is IDENTICAL to Leg V6
// (owner: perfect fit, 2026-11-27); the thigh is added around it without touching the pocket.
// Print pocket side UP, no supports.
use <leg-v6.scad>
use <leg-v7.scad>
$fn=72;
leg_t=8.5; pocket=2.2; pilot_r=13.4; pilot_d=2.0; pilot_depth=5.6; hip_d=14;   // copied from Leg V6
thigh_d=32;
module thigh() difference() {
  cylinder(d=thigh_d,h=leg_t);
  translate([0,0,-1]) cylinder(d=hip_d-0.01,h=leg_t+2);                         // Leg V6 hip fills this
  translate([0,0,leg_t-pocket]) linear_extrude(pocket+1) offset(0.01) arm_outline(); // horn pocket stays open
  for(s=[-1,1]) translate([0,s*pilot_r,leg_t-pocket-pilot_depth]) cylinder(d=pilot_d,h=pilot_depth+1);
}
leg7();
thigh();
assert(thigh_d/2>=sqrt(pow(11.5,2)+pow(8.5,2))+1,"Thigh covers the body U-slot (Puppy V2 shell)");
