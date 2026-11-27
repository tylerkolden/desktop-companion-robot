// Dog body V2: Dog body V1 with the four deck posts moved in to x=+/-10 (puppy V2 layout).
// Everything else is the same as V1. Holds four SG90 servos for checkpoint 6. Units mm. OpenSCAD 2021.01.
// A flat deck with two side walls. Each wall has two openings that copy the
// owner-confirmed V4 mount: 23 x 12.9 opening, two M2 screw slots, cable gap.
// Servos go in from OUTSIDE: ears rest on the outer face, body pokes inside,
// shaft points out sideways. Legs (Leg V6) hang outside the walls.
// The electronics ride on dog-deck-v3.scad, screwed to the four posts here.
// The MPU6050 sits on the floor in the middle, near the dog's centre (best for tilt readings).
// Print deck-down, no supports (the openings bridge 23mm, like a doorway).
$fn=48;

// --- copied from servo-cradle-v4.scad (owner print: screws fit) ---
body_l=23; body_w=12.9; wall_t=4;          // wall thickness = V4 plate, so the same M2 x 10 stack works
cable_gap=6; slot_d=2.4; slot_in=13.8; slot_out=15.4;

// --- body layout ---
shaft_gap=90;        // front shaft to rear shaft; legs swing +/-45deg without touching
shaft_from_end=5.9;  // SG90 shaft centre from the near end of the case (seller drawing; not measured)
wall_h=30;           // like the V4 plate width
shaft_z=15;          // shaft height above the deck's underside
inner_w=40;          // between the walls; each servo pokes in about 12mm
deck_t=3;
body_len=120;
// --- deck posts: blocks inside the walls between the servos ---
post_x=10; post_l=7; post_in=5; pilot_d=2.0; pilot_depth=10;   // big servo-bag screws, same pilot as Leg V6
// --- MPU6050 (GY-521) on the floor: owner photo about 21 x 16, two holes on the edge away from the pins ---
mpu=[21,16]; mpu_hole=15.5; mpu_hole_in=2.5; mpu_post_h=3; mpu_post_d=6; m2_free=3.4; nut_flat=4.0+0.6; nut_h=1.8;
open_cx=shaft_gap/2-(body_l/2-shaft_from_end);   // opening centre; shaft end faces the front/back

module capsule(x1,x2,d,h) { hull() for(x=[x1,x2]) translate([x,0,0]) cylinder(d=d,h=h); }

module wall_cuts() {   // in wall coordinates: x along body, z up, y through the wall
  for(sx=[-1,1]) {
    cx=sx*open_cx;
    translate([cx-body_l/2,-1,shaft_z-body_w/2]) cube([body_l,wall_t+2,body_w]);
    translate([cx-cable_gap/2,-1,shaft_z]) cube([cable_gap,wall_t+2,wall_h]);    // cable gap up to the top edge
    for(side=[-1,1]) translate([cx,-1,shaft_z]) rotate([-90,0,0]) capsule(side*slot_in,side*slot_out,slot_d,wall_t+2);
  }
}
// MPU long side along the body, pins on the -y side, holes on the +y side
module mpu_holes() { for(sx=[-1,1]) translate([sx*mpu_hole/2, mpu[1]/2-mpu_hole_in, 0]) children(); }
module mpu_posts() { mpu_holes() translate([0,0,deck_t]) cylinder(d=mpu_post_d,h=mpu_post_h); }
module mpu_cut() { mpu_holes() { translate([0,0,-1]) cylinder(d=m2_free,h=deck_t+mpu_post_h+2); translate([0,0,-1]) cylinder(d=nut_flat/cos(30),h=nut_h+1,$fn=6); } }
module body() {
  difference() {
    union() {
      translate([-body_len/2,-(inner_w/2+wall_t),0]) cube([body_len,inner_w+2*wall_t,deck_t]);
      for(sy=[-1,1]) translate([-body_len/2, sy>0 ? inner_w/2 : -inner_w/2-wall_t, 0]) cube([body_len,wall_t,wall_h]);
      for(sx=[-1,1],sy=[-1,1]) translate([sx*post_x-post_l/2, sy>0 ? inner_w/2-post_in : -inner_w/2, 0]) cube([post_l,post_in+0.01,wall_h]);
      mpu_posts();
    }
    for(sy=[-1,1]) translate([0, sy>0 ? inner_w/2 : -inner_w/2-wall_t, 0]) wall_cuts();
    for(sx=[-1,1],sy=[-1,1]) translate([sx*post_x,sy*(inner_w/2+1),wall_h-pilot_depth]) cylinder(d=pilot_d,h=pilot_depth+1);
    mpu_cut();
  }
}
body();

leg_reach=45+5;      // shaft to bottom of the Leg V6 foot
assert(open_cx+slot_out+slot_d/2+4<=body_len/2,"Wall material beyond the outer screw slots");
assert(open_cx-body_l/2-cable_gap>=0,"Front and rear openings stay apart");
assert(shaft_gap-2*leg_reach*sin(45)>=15,"Front and rear feet stay 15mm apart at +/-45deg");
assert(shaft_z-body_w/2>=deck_t+4,"Opening clears the deck");
assert(leg_reach-shaft_z>=30,"Deck is 30mm+ above the floor when standing");
assert(post_x+post_l/2<open_cx-body_l/2-1,"Deck posts clear of the servo bodies");
assert(inner_w/2-post_in>mpu[1]/2+1,"MPU clear of the posts");
assert(inner_w/2+1-pilot_d/2-1>=inner_w/2-post_in && inner_w/2+1+pilot_d/2+1<=inner_w/2+wall_t,"Pilot has 1mm+ of plastic around it");
echo(stand_height_deck=leg_reach-shaft_z, shaft_x=shaft_gap/2, opening_centre=open_cx);
