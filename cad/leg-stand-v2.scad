// Leg test stand V2. Units mm. OpenSCAD 2021.01.
// V1 (owner print, 2026-11-27): the mount sat loose in the 4.3mm slot and leaned badly.
// The owner's mount plate thickness is not measured, so V2 does not guess it:
// the slot is wider and two small printed WEDGES are pushed in beside the plate, one at each
// end of the slot, to clamp it tight. They sit at the ends so they miss the servo body and the
// ear screws, which stick out of the plate just above the tower top.
// The wedge handles plates from about 2.2 to 4.7mm thick.
// part="stand" or part="wedge". Print both flat, no supports.
$fn=48;
part="stand";

plate_l=44;          // mount plate long edge (V4 design)
slot_w=6.0;          // plate + wedge
slot_depth=8;        // deeper would hit the servo body that sticks through the plate
wall=4;              // thicker walls than V1 so the slot does not spread
tower_h=58;          // same shaft height as V1 (about 65mm above the table)
base=[80,60,4];

wedge_h=14;          // 8 in the slot, 6 above it to grip and pull out
wedge_thin=0.8;      // bottom edge
wedge_thick=3.8;     // top edge
wedge_l=4.5;         // each wedge; goes in the slot ends, |x| from about 17.5 to 22

module stand() {
  difference() {
    union() {
      translate([-base[0]/2,-base[1]/2,0]) cube(base);
      translate([-(plate_l+2*wall)/2,-(slot_w+2*wall)/2,0]) cube([plate_l+2*wall,slot_w+2*wall,tower_h]);
    }
    translate([-(plate_l+0.8)/2,-slot_w/2,tower_h-slot_depth]) cube([plate_l+0.8,slot_w,slot_depth+.1]);
  }
}
// Lies flat on the bed: x = length, y = height (thin end at y=0), z = thickness.
module wedge() {
  rotate([90,0,0]) rotate([0,90,0])
    linear_extrude(wedge_l) polygon([[0,0],[wedge_h,0],[wedge_h,wedge_thick],[0,wedge_thin]]);
}

if(part=="stand") stand(); else wedge();   // print TWO wedges (set copies to 2 in the slicer)

assert(slot_w-wedge_thick>=2.0 && slot_w-wedge_thin<=5.4,"Wedge covers plates about 2.2-4.7mm with printer shrink");
assert(tower_h-slot_depth+15-45-5>base[2]+8,"Foot clears the base");
