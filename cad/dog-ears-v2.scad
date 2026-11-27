// Dog ears V2 (Puppy V2). Print 2, flat, no supports. Try a different colour!
// Rounder and softer than V1: a big round flap that hangs from one screw. The M2 x 10 screw
// goes through the top hole, through the head and into the chin post. Flip one over for a pair.
include <dog-puppy2-common.scad>
$fn=48;
ear_t=3;
difference() {
  linear_extrude(ear_t) hull() { circle(d=10); translate([2,-16]) circle(d=27); }
  translate([0,0,-1]) cylinder(d=m2_free,h=ear_t+2);
}
