// Dog deck V2 (puppy): screws on top of Dog body V1 with 4 big servo-bag screws.
// Carries the PCA9685 (6-pin header pointing FORWARD, toward the neck), the hollow neck that
// holds the head, and two rear posts. The shell rests on the neck shoulder and the rear posts;
// the head sits on top of the shell and clamps it down.
// The XIAO, breadboard and OLED ride in the head; the MPU6050 stays on the body floor.
// Replaces deck V1 (never printed). Print deck-down, no supports.
// Board sizes from the owner's grid photos (about +/-1mm).
include <dog-puppy-common.scad>
$fn=40;
body_x=[-60,62]; body_w=48; screw_x=20; screw_y=21;
pca=[62.5,25.4]; pca_x0=-58.5; pca_hole=[57.2,19.8]; post_h=3; post_d=6.5; m2_hole=3.4; nut_flat=4.6; nut_h=1.8;
header_plug=22;                         // 6-pin header (8) + Dupont plugs (14) in front of the PCA
hole=[[6,24],12];                       // servo cables and MPU wires go down here
rest=[[-52,-46],[15,20]];               // rear posts the shell rests on (both sides)
pcx=pca_x0+pca[0]/2;
module tube(xr,hw,z0,z1,w) difference() {
  translate([xr[0],-hw,z0]) cube([xr[1]-xr[0],2*hw,z1-z0]);
  translate([xr[0]+w,-hw+w,z0-1]) cube([xr[1]-xr[0]-2*w,2*hw-2*w,z1-z0+2]);
}
module under_roof() translate([0,0,-deck_z]) shell_solid(wall+0.2);   // whole-dog shell, moved into this part's z
module deck() {
  difference() {
    union() {
      translate([body_x[0],-body_w/2,0]) cube([body_x[1]-body_x[0],body_w,deck_t]);
      for(sx=[-1,1],sy=[-1,1]) translate([pcx+sx*pca_hole[0]/2,sy*pca_hole[1]/2,deck_t]) cylinder(d=post_d,h=post_h);
      intersection() {
        union() {
          tube(neck_lo[0],neck_lo[1],deck_t,60,neck_wall);
          for(sy=[-1,1]) translate([rest[0][0], sy>0 ? rest[1][0] : -rest[1][1], deck_t]) cube([rest[0][1]-rest[0][0],rest[1][1]-rest[1][0],60]);
        }
        under_roof();
      }
      tube(neck_hi[0],neck_hi[1],deck_t,chin_z-deck_z,neck_wall);
    }
    for(sx=[-1,1],sy=[-1,1]) translate([pcx+sx*pca_hole[0]/2,sy*pca_hole[1]/2,-1]) {
      cylinder(d=m2_hole,h=deck_t+post_h+2); cylinder(d=nut_flat/cos(30),h=nut_h+1,$fn=6); }
    for(sx=[-1,1],sy=[-1,1]) translate([sx*screw_x,sy*screw_y,-1]) cylinder(d=m2_free,h=deck_t+2);
    translate([hole[0][0],-hole[1],-1]) cube([hole[0][1]-hole[0][0],2*hole[1],deck_t+2]);
    translate([neck_lo[0][0]-1,-8,deck_t]) cube([neck_wall+2,16,16]);                       // wires out of the neck to the PCA
    for(sy=[-1,1]) translate([(neck_hi[0][0]+neck_hi[0][1])/2,sy*(neck_hi[1]-neck_wall/2),chin_z-deck_z-10]) cylinder(d=m2_pilot,h=11); // chin screws
  }
}
deck();
assert(pca_x0>=body_x[0]+1,"PCA on the deck");
assert(pca_x0+pca[0]+header_plug<=neck_lo[0][0],"Room in front of the PCA for its header plugs");
assert(neck_lo[0][0]>hole[0][1],"Wire hole behind the neck");
assert(pca[1]/2+clr<screw_y-3 && pca[1]/2+1<rest[1][0],"PCA clear of the deck screws and rear posts");
