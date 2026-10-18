// Screw-free cable-entry prototype. Units mm. No measured strength claim.
// Body opening retains the owner's successful two-dot gauge dimensions.
$fn=48;
body_l=23; body_w=12.9; plate=3;
cable_gap=6; ear_clearance=2.9;
// Four in-plane cantilevers: 12mm free length, 1.2mm bending thickness.
module finger(sx,sy) {
 mirror([sx<0?1:0,0,0]) mirror([0,sy<0?1:0,0]) {
  translate([2,7.6,0]) cube([14,1.2,plate]);
  translate([13,7.6,0]) cube([3,1.2,7.2]);
  // Flat retaining underside, sloped lead-in on top; short 1.9mm overhang.
  translate([13,0,0]) rotate([90,0,90]) linear_extrude(3)
   polygon([[5.7,5.9],[8.8,5.9],[8.8,7.2],[7,7.2],[5.7,5.9]]);
 }
}
union() {
 difference() {
  linear_extrude(plate) offset(r=2) offset(delta=-2) square([42,28],center=true);
  translate([-body_l/2,-body_w/2,-.1]) cube([body_l,body_w,plate+.2]);
  // Side-entry channel: cable drops in sideways; connector stays attached.
  translate([body_l/2-.1,-cable_gap/2,-.1]) cube([10,cable_gap,plate+.2]);
  // Isolate flexible beams from the frame, leaving their central roots attached.
  for(sx=[-1,1],sy=[-1,1])
   mirror([sx<0?1:0,0,0]) mirror([0,sy<0?1:0,0])
    translate([4,6.45,-.1]) cube([13.5,4.5,plate+.2]);
 }
 for(sx=[-1,1],sy=[-1,1]) finger(sx,sy);
}
assert(body_l==23 && body_w==12.9);
assert(plate+ear_clearance==5.9);
