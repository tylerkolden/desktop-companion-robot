// Leg V7: Leg V6 with a round paw at the foot (puppy look). The hip end is IDENTICAL to
// Leg V6 (owner: perfect fit, 2026-11-27); only the foot is wider. Print pocket side UP, no supports.
use <leg-v6.scad>
$fn=64;
leg_t=8.5; leg_len=45; paw=[18,11];
module paw2d() { translate([0,-leg_len]) hull() { circle(d=10); translate([0,-0.5]) scale([1,paw[1]/paw[0]]) circle(d=paw[0]); } }
module leg7() { leg(); linear_extrude(leg_t) paw2d(); }
leg7();
