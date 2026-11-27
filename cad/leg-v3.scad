// Leg V3: Leg V2 with a sloped hub recess. Shaped around the owner's measured double-arm horn. Units mm. OpenSCAD 2021.01.
// Horn measured 2026-11-27 from grid-sheet photos (about +/-0.2mm), not calipers:
//   overall length 35.7, hub boss 7.2 dia x 2.0 tall (spline side),
//   screw-side bump is NOT a flat disc (Andrew spotted this, side photos 2026-11-27):
//   it slopes up from about 9.0 dia at the arm to a 5-6 dia top, about 1.1 tall.
//   Owner's printed gauge: arm 2.0 thick, spline boss 7.5.
//   arm 2.0 thick,
//   arm 6.0 wide beside the hub tapering to 3.6 at the rounded tip,
//   arm holes about 1.4 dia at 5.9, 8.4, 10.9, 13.4, 15.9 from centre.
// The horn stays as mounted on the servo: tall spline boss DOWN onto the motor, sloped bump UP into this leg pocket. The leg sits on top like a lid.
// An M2 x 6 screw through the leg and horn into the servo shaft clamps everything.
// Print pocket side UP. No supports.
$fn=64;

horn_len=35.7; arm_t=2.0; arm_w_hub=6.0; arm_w_tip=3.6; hub_x=3.5;
bump_base=9.0; bump_top=5.5; bump_h=1.1;   // measured sloped bump
rc_base=bump_base+0.6; rc_top=bump_top+0.9; rc_h=bump_h+0.2;  // recess: same slope, a little bigger
pilot_r=13.4;     // 4th hole from the centre, both arms
clr=0.4;          // total extra width/length in the pocket (0.2 each side)

leg_t=5.3; hip_d=13; foot_d=10; leg_len=45; tail_d=8; tail_len=16;
pocket=arm_t;                 // arm sits flush with the leg face
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
    translate([0,0,leg_t-recess]) cylinder(d1=rc_top,d2=rc_base,h=rc_h+.01);   // bowl: narrow at the bottom, wide at the arm
    translate([0,0,leg_t-pocket]) cylinder(d=rc_base,h=pocket+.1);
    translate([0,0,-.1]) cylinder(d=2.4,h=leg_t+.2);              // M2 shaft screw
    for(s=[-1,1]) translate([0,s*pilot_r,leg_t-pocket-2.5]) cylinder(d=1.0,h=2.6); // optional tiny horn screws
    translate([-2,-leg_len-foot_d/2-1,-.1]) cube([4,4,0.7]);      // toe mark on the bed face
  }
}
leg();

assert(floor_t>=2,"Plastic under the hub for the clamp screw");
assert(tail_len+tail_d/2-(horn_len+clr)/2>=1.5,"Wall beyond the upper horn tip");
assert((hip_d-rc_base)/2>=1.6,"Side wall around the hub recess");
assert(leg_t-pocket-2.5>=0.4,"Pilot holes stay inside the leg");
