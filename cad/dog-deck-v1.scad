// Dog top deck V1: carries the electronics on top of Dog body V1. Units mm. OpenSCAD 2021.01.
// Rear to front: mini breadboard (XIAO plugs in, USB-C points out the back like a tail),
// wire hole, PCA9685 on four short posts, OLED face standing up at the front.
// The MPU6050 does NOT go here: it sits on the body floor at the centre (dog-body-v1.scad).
// Board sizes are from the owner's grid-sheet photos (about +/-1mm), not calipers.
// Print flat side down (deck on the bed). The face window bridges 24mm; no supports.
$fn=40;

// --- body it sits on (copied from dog-body-v1.scad) ---
body_len=120; body_w=48; screw_y=21;         // body pilot holes: 1mm outside the inner wall face
open_cx=39.4; cable_gap=6;                    // servo cable gaps in the walls
post_x=20;                                    // body posts that take the deck screws
notch=4.5;                                    // cable notch depth: the 4mm wall plus a little

// --- deck ---
deck_t=3;
deck_x0=-73; deck_x1=76;                      // longer than the body: hangs 13-16mm over each end
clr=0.5;                                      // per side; this printer makes holes small
deck_screw_d=2.9;                             // clearance for the big servo-bag self-tapping screws

// --- PCA9685 (photo: 62.5 x 25.4; holes about 2.6mm in from each edge) ---
pca=[62.5,25.4]; pca_x0=2;                    // rear edge of the board; its 6-pin header points back
pca_hole=[57.2,19.8];                         // centre to centre, estimated from photo
pca_post_h=3; pca_post_d=6.5; m2_free=3.4;    // 3.4mm holes forgive about 0.7mm of hole-spacing error
nut_flat=4.0+0.6; nut_h=1.8;                  // M2 nut trap under the deck

// --- mini breadboard (170-point, about 47 x 35, sticky back) ---
bb=[47,35]; bb_x0=deck_x0+1; rim_h=2; rim_t=1.2;

// --- wire hole between breadboard and PCA (MPU wires come up here) ---
hole_x0=bb_x0+bb[0]+2*clr+rim_t+1; hole_x1=pca_x0-1; hole_w=26;

// --- OLED face (owner fit gauge: 27.5 x 27.8 board) ---
oled=[27.5,27.8]; oled_t=4.5;                 // slot front-to-back: glass + board + parts
plate_t=2; rail_t=2; rail_cover=2.5; ledge_h=2;
win=[24,16];                                  // window for the glass; screen area is about 22 x 11
face_x1=deck_x1; face_x0=face_x1-plate_t-oled_t-2*clr-rail_t;
face_h=ledge_h+oled[1]+clr+1;

module pca_posts_add() {
  for(sx=[-1,1],sy=[-1,1]) translate([pca_x0+pca[0]/2+sx*pca_hole[0]/2, sy*pca_hole[1]/2, deck_t])
    cylinder(d=pca_post_d,h=pca_post_h);
}
module pca_posts_cut() {
  for(sx=[-1,1],sy=[-1,1]) translate([pca_x0+pca[0]/2+sx*pca_hole[0]/2, sy*pca_hole[1]/2, 0]) {
    translate([0,0,-1]) cylinder(d=m2_free,h=deck_t+pca_post_h+2);
    translate([0,0,-1]) cylinder(d=nut_flat/cos(30),h=nut_h+1,$fn=6);   // nut sits here from below
  }
}
module breadboard_rim() {
  w=bb[0]+2*clr; d=bb[1]+2*clr;
  translate([bb_x0,-d/2-rim_t,deck_t]) difference() {
    cube([w+rim_t,d+2*rim_t,rim_h]);
    translate([-1,rim_t,-1]) cube([w+1,d,rim_h+2]);   // open at the back so the USB cable can come out
  }
}
module face() {
  w=oled[0]+2*clr;
  translate([face_x0,-w/2-rail_t,0]) difference() {
    cube([face_x1-face_x0,w+2*rail_t,deck_t+face_h]);
    // slot the OLED drops into from the top
    translate([rail_t,rail_t,deck_t+ledge_h]) cube([oled_t+2*clr,w,face_h]);
    // open back between the rails, so the pins and wires come out
    translate([-1,rail_t+rail_cover,deck_t+ledge_h]) cube([rail_t+2,w-2*rail_cover,face_h]);
    // window in the front plate
    translate([rail_t+oled_t+2*clr-1,rail_t+w/2-win[0]/2,deck_t+ledge_h+oled[1]/2-win[1]/2]) cube([plate_t+2,win[0],win[1]]);
  }
}
module deck() {
  difference() {
    union() {
      translate([deck_x0,-body_w/2,0]) cube([deck_x1-deck_x0,body_w,deck_t]);
      pca_posts_add();
      breadboard_rim();
      face();
    }
    pca_posts_cut();
    translate([hole_x0,-hole_w/2,-1]) cube([hole_x1-hole_x0,hole_w,deck_t+2]);
    for(sx=[-1,1],sy=[-1,1]) translate([sx*post_x,sy*screw_y,-1]) cylinder(d=deck_screw_d,h=deck_t+2);
    // notches above the servo cable gaps so the cables come up beside the deck
    for(sx=[-1,1],sy=[-1,1]) translate([sx*open_cx-(cable_gap+2)/2, sy>0 ? body_w/2-notch : -body_w/2-1, -1]) cube([cable_gap+2,notch+1,deck_t+2]);
  }
}
deck();

assert(pca_x0-(bb_x0+bb[0]+2*clr+rim_t)>=24,"Room behind the PCA for its 6 header plugs (pins 8 + plug 14)");
assert(pca_x0+pca[0]+1<=face_x0,"PCA clear of the face");
assert(pca[1]/2+clr<body_w/2-notch,"PCA clear of the cable notches");
assert(bb[1]/2+clr+rim_t<=body_w/2-notch,"Breadboard rim clear of the cable notches");
assert(pca_post_h>=2.5,"PCA solder tails clear the deck");
assert(win[0]<=oled[0]-3 && win[1]<=oled[1]-6,"Front plate overlaps the board so it cannot fall out the front");
echo(deck_len=deck_x1-deck_x0, wire_hole=[hole_x0,hole_x1], face_x=[face_x0,face_x1], face_top=deck_t+face_h);
