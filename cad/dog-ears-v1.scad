// Dog ears V1 (puppy). Print 2, flat, no supports. Try a different colour!
// A floppy ear: the M2 x 10 screw goes through the top hole, through the head and into the
// chin post. The ear hangs about 31mm below the screw. Flip one over so they make a pair.
include <dog-puppy-common.scad>
$fn=40;
ear_t=3;
difference() {
  linear_extrude(ear_t) hull() { circle(d=11); translate([3,-19]) scale([1,1.2]) circle(d=22); }
  translate([0,0,-1]) cylinder(d=m2_free,h=ear_t+2);
}
