// WWZMDiB SG90 first cradle prototype. Millimeters.
// Opening from owner-confirmed two-dot gauge; see SERVO-DIMENSIONS.md.
// Print flange flat on bed. Turn over for use: ears rest on flat flange face.
$fn=48;
body_length=23.0;
body_width=12.9;
flange_length=42;
flange_width=24.9;
flange_thickness=3;
rail_thickness=3;
rail_depth=7;
slot_width=2.2; // prototype clearance, NOT a verified screw specification
slot_inner_center=13.8;
slot_outer_center=15.4;
module slot(side) {
 hull() for(x=[slot_inner_center,slot_outer_center])
  translate([side*x,0,-0.1]) cylinder(d=slot_width,h=flange_thickness+0.2);
}
difference() {
 union() {
  linear_extrude(flange_thickness)
   offset(r=2) offset(delta=-2) square([flange_length,flange_width],center=true);
  // Two short guide walls; open ends and bottom leave cable access.
  for(side=[-1,1])
   translate([-body_length/2,side>0?body_width/2:-body_width/2-rail_thickness,flange_thickness-0.1])
    cube([body_length,rail_thickness,rail_depth+0.1]);
 }
 translate([-body_length/2,-body_width/2,-0.1])
  cube([body_length,body_width,flange_thickness+rail_depth+0.2]);
 slot(-1); slot(1);
}
assert(body_length==23 && body_width==12.9,"Preserve confirmed two-dot fit");
assert(slot_inner_center-slot_width/2>body_length/2+1,"Keep material between slot and opening");
