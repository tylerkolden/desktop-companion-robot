// Shared numbers for Puppy V2 (checkpoint 6). Units mm, whole-dog coordinates:
// x = forward, y = left, z = up, z=0 is the underside of Dog body V2 (dog-body-v2.scad).
// V2 changes from V1: the legs are OUTSIDE a slimmer body so they can swing 90 deg and the dog
// can lie flat; the head is small with a big screen face; the breadboard moved into the body.
// Included by dog-deck-v3, dog-shell-v2, dog-head-v2, dog-ears-v2 and dog-v2-assembly.
deck_z=30; deck_t=3; deck_top=deck_z+deck_t;    // deck sits on the 30mm body walls
clr=0.5;                                        // per side; this printer makes holes small
m2_pilot=1.7;                                   // M2 machine screw cuts its own thread in PLA
m2_free=2.9;                                    // M2 passes through freely (Leg V4 lesson)
nut_flat=4.6; nut_h=1.8;                        // M2 nut trap
wall=2;                                         // shell and head wall

// SG90 sticking out of the body wall (wall face y=24), from the seller drawing, NOT measured:
case_top_y=28.3; gear_top_y=32.3; leg_in_y=37; leg_out_y=45.5; shaft_x=45; shaft_z=15;

// body shell: a long round capsule, narrower than the legs. U-slots open at the bottom let it
// drop over the servo gear towers.
shell=[150,64]; shell_bot=4; shell_top=66;
shell_rp=24;            // round ends seen from above
shell_rt=26;            // how far down the top edge curls
gear_slot_r=8.5;        // around each servo shaft
gear_slot_back=3;       // the slot also reaches this far toward the middle (second gear bump)

// head: small and round, so the screen is a big part of the face. It sits straight on the body.
head=[50,56]; head_cx=50; head_bot=58; head_top=112;
head_R=22; face_w=46; face_z=[66,108]; face_r=12;
chin_z=66; chin_t=3; chin_top=chin_z+chin_t;    // chin plate rests on the flat top of the body
chin_screw=[[34,-6],[34,6]];                    // M2 x 10 + nut: chin plate to body roof
wire_hole=[[44,54],3.5];                        // OLED wires: x range, half width in y
ear_x=42; ear_z=99;

module rounded_block(size,z0,z1,rp,rt) hull() for(sx=[-1,1],sy=[-1,1])
  translate([sx*(size[0]/2-rp),sy*(size[1]/2-rp),0]) {
    translate([0,0,z0]) cylinder(r=rp,h=z1-rt-z0);
    translate([0,0,z1-rt]) scale([1,1,rt/rp]) sphere(rp);
  }
module shell_solid(o=0) rounded_block([shell[0]-2*o,shell[1]-2*o],shell_bot-(o>0?1:0),shell_top-o,shell_rp-o,shell_rt-o);
// shell footprint seen from above, shrunk by o (used for the deck outline)
module shell_plan(o=0) hull() for(sx=[-1,1],sy=[-1,1]) translate([sx*(shell[0]/2-shell_rp),sy*(shell[1]/2-shell_rp)]) circle(shell_rp-o);
module head_solid(o=0) intersection() {
  hull() {
    for(sy=[-1,1],z=[head_top-head_R, head_bot+head_R*0.6]) translate([head_cx-head[0]/2+head_R,sy*(head[1]/2-head_R),z]) sphere(head_R-o);
    translate([head_cx+head[0]/2-o-0.01,0,0]) rotate([0,90,0]) linear_extrude(0.01) hull()
      for(sy=[-1,1],z=[face_z[0]+face_r, face_z[1]-face_r]) translate([-z,sy*(face_w/2-face_r)]) circle(face_r-o);
  }
  translate([-200,-200,head_bot-(o>0?1:0)]) cube([400,400,200]);
}
module head_outer() difference() { head_solid(0); shell_solid(-clr); }
// top of the shell's inside at (x,y): used to check boards fit under the roof
function roof_in(y) = let(o=wall, cy=shell[1]/2-shell_rp, r=shell_rp-o, dy=max(0,abs(y)-cy))
  dy>=r ? -1 : shell_top-shell_rt + (shell_rt-o)*sqrt(1-pow(dy/r,2));

// Tail servo (added 2026-11-28, Puppy V2 + tail): an SG90 lying with its shaft pointing BACKWARD
// through the rear of the shell, long side up and down. The tail turns around the shaft, so it
// wags side to side. Held by a V4-style plate at the back of Dog body V3.
tail_case_top_x=-74.5;              // case top (shaft side), from the SG90 drawing, not measured
tail_ear_x=tail_case_top_x+4.3;     // ears rest on the back face of the plate (like the body walls)
tail_plate_t=4;
tail_zc=17.5;                       // middle of the servo case (height). Was 19: the case top touched the deck (pre-print check 2026-11-28)
tail_shaft_z=tail_zc+11.5-5.9;      // shaft is 5.9 from the top end of the case
tail_gear_x=tail_case_top_x-4;      // gear tower tip
tail_face_x=tail_gear_x-4.7;        // tail part's pocket face (same gap as the legs: gear top to leg face)
