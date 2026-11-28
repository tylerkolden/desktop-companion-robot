// Shared numbers for the puppy look (checkpoint 6). Units mm, whole-dog coordinates:
// x = forward, y = left, z = up, z=0 is the underside of Dog body V1 (dog-body-v1.scad).
// Included by dog-deck-v2, dog-shell-v1, dog-head-v1, dog-ears-v1 and the assembly render.
deck_z=30; deck_t=3; deck_top=deck_z+deck_t;    // deck sits on the 30mm body walls
clr=0.5;                                        // per side; this printer makes holes small
m2_pilot=1.7;                                   // M2 machine screw cuts its own thread in PLA
m2_free=2.9;                                    // M2 passes through freely (Leg V4 lesson)
wall=2;                                         // shell and head wall

// body shell: one big round bean that hides the frame, servos and hips. Open belly: only the
// lower legs poke out underneath. It is a hull of four big balls, cut flat at the bottom.
shell=[150,120]; shell_bot=2; shell_top=68;
shell_R=41;
// leg points that must stay inside (from Leg V6 at +/-40 deg; leg y from the SG90 drawing, not measured)
leg_out_y=46; leg_top=[58,35]; hip=[52,8];   // [x,z] of the upper-tail tip and the hip disc bottom-front
servo_top=21.5;

// neck: a hollow tube on the deck. The shell rests on its shoulder; the head sits on its top.
neck_lo=[[26,50],16];   // x range, half width (below the shell roof)
neck_hi=[[28,48],12];   // x range, half width (through the hole in the shell roof)
neck_wall=3;

// head: round at the back and top, flat face at the front for the screen
// The head's lower edge is cut to hug the body, so it sits snugly on the shoulders.
head=[80,82]; head_cx=62; head_bot=56; head_top=140;
head_R=30; face_w=52; face_z=[82,122]; face_r=14;   // flat face: just big enough for the screen
chin_z=73; chin_t=3;    // flat plate inside the head, on top of the neck
ear_x=44; ear_z=110;

module shell_solid(o=0) intersection() {
  hull() for(sx=[-1,1],sy=[-1,1]) translate([sx*(shell[0]/2-shell_R),sy*(shell[1]/2-shell_R),shell_top-shell_R]) sphere(shell_R-o);
  translate([-200,-200,shell_bot-(o>0?1:0)]) cube([400,400,200]);
}
module head_solid(o=0) intersection() {
  hull() {
    for(sy=[-1,1],z=[head_top-head_R, head_bot+head_R*0.4]) translate([head_cx-head[0]/2+head_R,sy*(head[1]/2-head_R),z]) sphere(head_R-o);
    // flat face: a rounded rectangle standing at the front
    translate([head_cx+head[0]/2-o-0.01,0,0]) rotate([0,90,0]) linear_extrude(0.01) hull()
      for(sy=[-1,1],z=[face_z[0]+face_r, face_z[1]-face_r]) translate([-z,sy*(face_w/2-face_r)]) circle(face_r-o);
  }
  translate([-200,-200,head_bot-(o>0?1:0)]) cube([400,400,200]);
}
module head_outer() difference() { head_solid(0); shell_solid(-clr); }
