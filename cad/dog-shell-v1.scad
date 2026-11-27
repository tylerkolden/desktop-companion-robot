// Dog shell V1 (puppy body): a round bean with a curly tail, lowered straight down over the deck.
// The neck pokes up through the hole in the roof. The shell rests on the neck shoulder and two
// rear posts, and the head sits on top of it and holds it down. Open belly.
// It hides the frame, servos and hips; only the lower legs poke out underneath.
// Print open side DOWN with tree supports (Bambu Studio: Support on, type tree(auto)).
include <dog-puppy-common.scad>
$fn=56;
tail_pts=[[-74,0,42],[-82,0,50],[-86,0,60],[-84,0,69],[-79,0,73]];
module tail() for(i=[0:len(tail_pts)-2]) hull() { translate(tail_pts[i]) sphere(d=11-i*1.3); translate(tail_pts[i+1]) sphere(d=11-(i+1)*1.3); }
module shell() {
  difference() {
    union() { shell_solid(0); tail(); }
    shell_solid(wall);
    translate([neck_hi[0][0]-clr,-neck_hi[1]-clr,50]) cube([neck_hi[0][1]-neck_hi[0][0]+2*clr,2*(neck_hi[1]+clr),40]);
  }
}
shell();
// distance from a point to the nearest ball centre must leave room inside the wall
function inside(x,y,z,m) = let(cx=shell[0]/2-shell_R, cy=shell[1]/2-shell_R, cz=shell_top-shell_R,
  d=sqrt(pow(max(0,abs(x)-cx),2)+pow(max(0,abs(y)-cy),2)+pow(z-cz,2))) d<=shell_R-wall-m;
assert(inside(leg_top[0],leg_out_y,leg_top[1],1),"Leg tops stay inside the body");
assert(inside(hip[0],leg_out_y,hip[1],1),"Hip discs stay inside the body");
assert(inside(62,24,30,1) && inside(-62,24,30,1),"Deck corners inside");
assert(inside(-50,12,60,1),"PCA plugs under the roof");
// leg swinging 40 deg must pass through the open belly
open_r=sqrt(pow(shell_R-wall,2)-pow(shell_top-shell_R-shell_bot,2));
assert(shell[0]/2-shell_R+open_r>=45+(15-shell_bot)*tan(40)+6,"Legs swing through the belly opening");
echo(belly_opening=[2*(shell[0]/2-shell_R+open_r),2*(shell[1]/2-shell_R+open_r)]);
