// Dog deck V3 (Puppy V2): screws on top of Dog body V2 with 4 M2 x 10 screws (they cut their own thread in the body posts).
// Rear: the PCA9685 (6-pin header pointing BACK, toward the tail).
// Front: the mini breadboard turned sideways, XIAO at the left end, USB-C out the left side.
// The MPU6050 stays on the body floor. Servo and MPU wires come up through the two rear slots.
// Four side blocks take the M2 screws that hold the body shell on.
// The outline follows the inside of the shell. Replaces deck V2 for Puppy V2.
// Print deck-down, no supports. Board sizes from the owner's grid photos (about +/-1mm).
include <dog-puppy2-common.scad>
$fn=40;
deck_x=[-62,62];
screw_x=10; screw_y=21;                 // matches post_x / pilots in dog-body-v2.scad
pca=[62.5,25.4]; pca_x0=-46; pca_hole=[57.2,19.8]; post_h=3; post_d=6.5; m2_hole=3.4;
header_plug=22;                         // 6-pin header (8) + Dupont plugs (14) behind the PCA
slot=[[-34,-24],[13.5,20]];             // wire slots, both sides (a servo plug fits through)
bb=[35,47]; rim_t=1.2; rim_h=2; bb_x0=18; // breadboard rim, open on the USB (+y) side
bb_w=bb[1]+2*clr+rim_t;
blk_x=[-13,13]; blk=[8,5.5,9];          // shell screw blocks (x centre, size), screw at blk_z
blk_z=deck_t+5; blk_y=shell[1]/2-wall-clr-blk[1];
pcx=pca_x0+pca[0]/2;
module deck() {
  difference() {
    union() {
      intersection() {
        translate([0,0,0]) linear_extrude(deck_t) shell_plan(wall+clr+0.2);
        translate([deck_x[0],-50,-1]) cube([deck_x[1]-deck_x[0],100,10]);
      }
      for(sx=[-1,1],sy=[-1,1]) translate([pcx+sx*pca_hole[0]/2,sy*pca_hole[1]/2,deck_t]) cylinder(d=post_d,h=post_h);
      translate([bb_x0,-bb[1]/2-clr-rim_t,deck_t]) difference() {
        cube([bb[0]+2*clr+2*rim_t,bb_w,rim_h]);
        translate([rim_t,rim_t,-1]) cube([bb[0]+2*clr,bb_w,rim_h+2]);
      }
      for(x=blk_x,sy=[-1,1]) translate([x-blk[0]/2, sy>0 ? blk_y : -blk_y-blk[1], deck_t]) cube([blk[0],blk[1],blk[2]]);
    }
    for(sx=[-1,1],sy=[-1,1]) translate([pcx+sx*pca_hole[0]/2,sy*pca_hole[1]/2,-1]) {
      cylinder(d=m2_hole,h=deck_t+post_h+2); cylinder(d=nut_flat/cos(30),h=nut_h+1,$fn=6); }
    for(sx=[-1,1],sy=[-1,1]) translate([sx*screw_x,sy*screw_y,-1]) cylinder(d=m2_free,h=deck_t+2);
    for(sy=[-1,1]) translate([slot[0][0], sy>0 ? slot[1][0] : -slot[1][1], -1]) cube([slot[0][1]-slot[0][0],slot[1][1]-slot[1][0],deck_t+2]);
    for(x=blk_x) translate([x,0,blk_z]) rotate([90,0,0]) cylinder(d=m2_pilot,h=80,center=true);
  }
}
deck();
inner_w=40;   // between the body walls
assert(pca_x0-header_plug>=-shell[0]/2+wall+5,"Room behind the PCA for its header plugs");
assert(pca_x0+pca[0]<=bb_x0-1,"PCA clear of the breadboard");
assert(pcx+pca_hole[0]/2+post_d/2<screw_x-m2_free/2-0.5 || pca[1]/2+clr<screw_y-3,"PCA clear of the deck screws");
assert(bb_x0>screw_x+3.5,"Breadboard clear of the deck screw heads");
assert(slot[1][0]>pca[1]/2 && slot[1][1]<=inner_w/2 && slot[0][1]<pcx+pca_hole[0]/2-post_d/2-1 && slot[0][0]>pcx-pca_hole[0]/2+post_d/2+1,"Wire slots beside the PCA, inside the walls, clear of its posts");
assert(blk_x[1]+blk[0]/2<bb_x0-0.5,"Front shell blocks clear of the breadboard");
assert(blk_y>bb_w/2-1 || blk_x[1]+blk[0]/2<bb_x0,"Shell blocks clear of the breadboard");
assert(roof_in(pca[1]/2)>=deck_top+post_h+1.6+20+2,"PCA servo plugs under the roof");
assert(roof_in(bb[1]/2)>=deck_top+8.5+12,"Breadboard wires under the roof");
echo(xiao_usb_y=bb[1]/2+clr, usb_z=deck_top+8.5+2.5+3.2/2+1, xiao_x=[bb_x0+rim_t+clr+(bb[0]-17.8)/2, bb_x0+rim_t+clr+(bb[0]+17.8)/2]);
