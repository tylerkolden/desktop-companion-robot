// Leg V6: uses only the screws from the servo bag (owner's discovery, 2026-11-27).
//   Large pointed screws (2): about 2.2 thread x 7 shank, flanged head about 4.7 dia x 1.6.
//     They go UP through the horn's 4th arm holes (from the motor side) into pilot holes in this leg.
//   Small screw (1): about 2.0 thread x 3.5 shank, head about 2.8. It holds the horn on the motor
//     shaft; a screwdriver reaches it through the big centre hole.
//   Screw sizes measured from grid-sheet photos, about +/-0.3mm.
// Clearances from Leg V4 (V3 printed too tight). Owner printed V3 (2026-11-27): horn and screws too tight.
// V3 layout: deeper disc recess plus sloped room for the ramps. Shaped around the owner's measured double-arm horn. Units mm. OpenSCAD 2021.01.
// Horn measured 2026-11-27 from grid-sheet photos (about +/-0.2mm), not calipers:
//   overall length 35.7, hub boss 7.2 dia x 2.0 tall (spline side),
//   screw-side bump (Andrew spotted this, photos 2026-11-27): a straight-sided disc,
//   about 8.0-8.7 dia x 1.1 tall, PLUS small ramps where it meets the arm. The ramps
//   rise from the arm face up to the disc top, so from the side the bump looks sloped.
//   Owner's printed gauge: arm 2.0 thick, spline boss 7.5.
//   arm 2.0 thick,
//   arm 6.0 wide beside the hub tapering to 3.6 at the rounded tip,
//   arm holes about 1.4 dia at 5.9, 8.4, 10.9, 13.4, 15.9 from centre.
// The horn stays as mounted on the servo: tall spline boss DOWN onto the motor, disc with ramps UP into this leg pocket. The leg sits on top like a lid.
// Order: horn into leg, 2 large screws, push onto shaft, small centre screw.
// Print pocket side UP. No supports.
$fn=64;

horn_len=35.7; arm_t=2.0; arm_w_hub=6.0; arm_w_tip=3.6; hub_x=3.5;
disc_d=8.7; bump_h=1.1;   // largest measured disc; height from the side photo
rc_d=disc_d+1.3;           // straight recess for the disc
rc_h=bump_h+0.4;           // a little deeper than the disc
ramp_d=13.4;               // ramp clearance: widens to this at the arm face, only inside the arm pocket
pilot_r=13.4;     // 4th hole from the centre, both arms
access_d=6.0;      // screwdriver + small screw head
pilot_d=2.0;       // for the 2.2mm self-tapping screws; this printer prints holes small
pilot_depth=5.6;   // 7mm shank minus 2mm horn, plus a little
clr=1.0;          // total extra width/length in the pocket (0.5 each side; V3 had 0.2 and was too tight)

leg_t=8.5; hip_d=14; foot_d=10; leg_len=45; tail_d=8; tail_len=16;
pocket=arm_t+0.2;             // arm sits just below the leg face
recess=pocket+rc_h;           // total depth to the bottom of the sloped recess
floor_t=leg_t-recess;         // plastic under the hub that the screw clamps
tip_c=horn_len/2-arm_w_tip/2;

module leg_outline() {
  hull() { circle(d=hip_d); translate([0,-leg_len]) circle(d=foot_d); }
  hull() { circle(d=hip_d); translate([0,tail_len]) circle(d=tail_d); }
}
module arm_outline() {
  hull() for(s=[-1,1]) {
    translate([0,s*hub_x]) square([arm_w_hub+clr,.01],center=true);
    translate([0,s*tip_c]) circle(d=arm_w_tip+clr);
  }
}
module leg() {
  difference() {
    linear_extrude(leg_t) leg_outline();
    translate([0,0,leg_t-pocket]) linear_extrude(pocket+.1) arm_outline();
    translate([0,0,leg_t-recess]) cylinder(d=rc_d,h=recess+.1);                 // straight-sided disc recess
    intersection() {                                                             // sloped room for the ramps
      translate([0,0,leg_t-recess]) cylinder(d1=rc_d,d2=ramp_d,h=rc_h+.01);
      translate([0,0,leg_t-recess-.1]) linear_extrude(rc_h+.2) arm_outline();
    }
    translate([0,0,-.1]) cylinder(d=access_d,h=leg_t+.2);         // reach the small centre screw
    for(s=[-1,1]) translate([0,s*pilot_r,leg_t-pocket-pilot_depth]) cylinder(d=pilot_d,h=pilot_depth+.1); // large servo-bag screws
    translate([-2,-leg_len-foot_d/2-1,-.1]) cube([4,4,0.7]);      // toe mark on the bed face
  }
}
leg();

assert(floor_t>=2,"Plastic under the hub for the clamp screw");
assert(tail_len+tail_d/2-(horn_len+clr)/2>=1.5,"Wall beyond the upper horn tip");
assert((hip_d-rc_d)/2>=1.6,"Side wall around the hub recess");
assert(leg_t-pocket-pilot_depth>=0.6,"Pilot holes stay blind: screw tips do not poke out");
