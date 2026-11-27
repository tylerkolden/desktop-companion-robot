// Leg V1 and single-leg test stand. Units mm. OpenSCAD 2021.01.
// part="leg":   one leg for the double-arm SG90 horn. Print pocket side UP.
// part="stand": holds one assembled V4 cradle upright so one leg swings freely.
// Horn size comes from the seller listing (33mm double arm), not a measured horn.
// Pocket clearance is deliberately generous; the horn's own screws do the holding.
$fn=64;
part="leg";

// ---------- leg ----------
leg_t=8;            // thick enough for the horn screws to bite without poking out
hip_d=14;           // width around the horn hub
foot_d=10;          // rounded foot
leg_len=45;         // shaft centre to foot centre
tail_len=15;        // covers the upper horn arm
horn_len=33;        // listing value: double-arm horn overall length
horn_hub_d=7.6;     // assumed hub size + clearance (not measured)
horn_tip_d=4.6;     // assumed arm tip width + clearance (not measured)
clear=0.6;          // total extra length/width in the pocket
pocket=1.4;         // pocket depth; the horn arm sits mostly inside it
access_d=5.5;       // screwdriver reaches the shaft screw through this hole
tip_c=(horn_len+clear)/2-(horn_tip_d+clear)/2;

module leg_outline() {
  hull() { circle(d=hip_d); translate([0,-leg_len]) circle(d=foot_d); }
  hull() { circle(d=hip_d); translate([0,tail_len]) circle(d=foot_d); }
}
module horn_pocket() {
  hull() {
    circle(d=horn_hub_d+clear);
    for(s=[-1,1]) translate([0,s*tip_c]) circle(d=horn_tip_d+clear);
  }
}
module leg() {
  difference() {
    linear_extrude(leg_t) leg_outline();
    translate([0,0,leg_t-pocket]) linear_extrude(pocket+.1) horn_pocket();
    translate([0,0,-.1]) cylinder(d=access_d,h=leg_t+.2);
    // Toe mark on the outer (bed) face: a shallow groove so you can see which end is the foot.
    translate([-2,-leg_len-foot_d/2-1,-.1]) cube([4,4,0.7]);
  }
}

// ---------- stand ----------
plate_t=4; plate_l=44;     // V4 cradle plate
slot_w=4.3; slot_depth=8;  // plate slides in on its continuous long edge
wall=2.5;
tower_h=58;                // puts the servo shaft about 65mm above the table
base=[80,60,4];
module stand() {
  difference() {
    union() {
      translate([-base[0]/2,-base[1]/2,0]) cube(base);
      translate([-(plate_l+2*wall)/2,-(slot_w+2*wall)/2,0])
        cube([plate_l+2*wall,slot_w+2*wall,tower_h]);
    }
    translate([-(plate_l+0.6)/2,-slot_w/2,tower_h-slot_depth])
      cube([plate_l+0.6,slot_w,slot_depth+.1]);
  }
}

if(part=="leg") leg(); else stand();

assert(tip_c+(horn_tip_d+clear)/2<=tail_len+foot_d/2-2.5,"Wall beyond upper horn tip");
assert(leg_t-pocket>=6,"Screw bite below the horn");
assert((hip_d-(horn_hub_d+clear))/2>=2.5,"Side wall around horn hub");
// Shaft height = slot bottom + half the 30mm plate; foot must clear the base.
assert(tower_h-slot_depth+15-leg_len-foot_d/2>base[2]+8,"Foot clears the base");
