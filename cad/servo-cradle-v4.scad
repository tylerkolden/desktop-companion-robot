// V4: screw-retained fit prototype. Units mm. No snap/friction retention.
// Print flat, same orientation used in assembly; ears rest on top at z=4.
// Side cable entry moved to LONG edge to preserve both screw bearing lands.
$fn=64;
body_l=23; body_w=12.9; plate=4;
outer_l=44; outer_w=30; cable_gap=6;
slot_d=2.4; slot_in=13.8; slot_out=15.4;
module capsule(x1,x2,d,h) {
 hull() for(x=[x1,x2]) translate([x,0,-.1]) cylinder(d=d,h=h+.2);
}
difference() {
 linear_extrude(plate)
  offset(r=3) offset(delta=-3) square([outer_l,outer_w],center=true);
 translate([-body_l/2,-body_w/2,-.1]) cube([body_l,body_w,plate+.2]);
 // Route only the loose wire bundle sideways; never pull the plug through.
 translate([-cable_gap/2,0,-.1]) cube([cable_gap,outer_w,plate+.2]);
 for(side=[-1,1]) capsule(side*slot_in,side*slot_out,slot_d,plate);
}
assert(body_l==23 && body_w==12.9,"Keep owner-confirmed aperture");
assert(slot_in-slot_d/2-body_l/2>=1,"Minimum inner slot ligament");
assert(outer_l/2-slot_out-slot_d/2>=5,"Outer screw bearing margin");
assert(outer_w/2-body_w/2>=8,"Continuous back rail width");
assert(cable_gap/2+5<slot_in-slot_d/2,"Cable entry stays away from fasteners");
