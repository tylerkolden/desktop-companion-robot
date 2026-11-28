// Dog body V3: Dog body V2 plus a plate at the back for the TAIL servo (Puppy V2 + tail).
// The tail SG90 goes in from behind, shaft pointing backward, long side up and down. Its ears
// rest on the back face of the plate and 2 M2 x 10 screws + nuts hold it, exactly like the V4
// cradle (same 23 x 12.9 opening, same screw slots, same cable gap). Everything else is Dog body V2.
// Print deck-down, no supports (the opening bridges 12.9mm).
include <dog-puppy2-common.scad>
use <dog-body-v2.scad>
$fn=48;
body_l=23; body_w=12.9; cable_gap=6; slot_d=2.4; slot_in=13.8; slot_out=15.4;   // from servo-cradle-v4.scad
px0=tail_ear_x; px1=px0+tail_plate_t; plate_w=30; plate_z=[0,tail_zc+17.6];
rib=[16,21];
module capsule(z1,z2,d,h) { hull() for(z=[z1,z2]) translate([0,0,z]) rotate([0,90,0]) cylinder(d=d,h=h); }
module tail_plate() difference() {
  union() {
    translate([px0,-plate_w/2,plate_z[0]]) cube([tail_plate_t,plate_w,plate_z[1]-plate_z[0]]);
    translate([px0,-plate_w/2,0]) cube([-60-px0+1,plate_w,3]);                       // floor joins the body floor
    for(sy=[-1,1]) translate([px1-0.01, sy>0 ? rib[0] : -rib[1], 0]) cube([-59-px1,rib[1]-rib[0],30]);   // side ribs into the walls
  }
  translate([px0-1,-body_w/2,tail_zc-body_l/2]) cube([tail_plate_t+2,body_w,body_l]);        // servo opening (long side up)
  translate([px0-1,0,tail_zc-cable_gap/2]) cube([tail_plate_t+2,plate_w,cable_gap]);         // cable gap to the +y edge
  for(s=[-1,1]) translate([px0-1,0,tail_zc]) capsule(s*slot_in,s*slot_out,slot_d,tail_plate_t+2);
  translate([px1,-body_w/2-0.5,3]) cube([-51.5-px1,body_w+1,tail_zc+body_l/2+1]);            // room for the servo body in the ribs/floor
}
union() { body(); tail_plate(); }
// rear leg servos poke in to |y|=5.3 from x=-50.9; the tail servo body must end before that
tail_case_bottom_x=tail_case_top_x+22.7;
assert(tail_case_bottom_x<=-50.9-0.8,"Tail servo clear of the rear leg servos");
assert(plate_z[1]<=deck_top+3+1.6-0.5,"Plate top 0.5mm+ below the PCA header plugs (they hang behind the PCA at board-top height)");
function in_shell(x,y,m) = let(cx=shell[0]/2-shell_rp, cy=shell[1]/2-shell_rp) sqrt(pow(max(0,abs(x)-cx),2)+pow(max(0,abs(y)-cy),2))<=shell_rp-wall-m;
assert(in_shell(px0,plate_w/2,clr+0.5) && in_shell(px1,rib[1],clr+0.5) && in_shell(px0,0,clr),"Tail plate and ribs inside the shell");
assert(plate_z[1]<40,"Plate below where the shell roof curls");
echo(tail_shaft=[tail_case_top_x,0,tail_shaft_z], plate=[px0,px1,plate_z], case_bottom=tail_case_bottom_x);
