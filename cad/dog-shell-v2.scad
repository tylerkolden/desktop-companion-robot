// Dog shell V2 (Puppy V2 body): a slim round capsule with a curly tail, lowered straight down
// over the deck. It is narrower than the legs: the legs and their round thighs stay OUTSIDE,
// so they can swing 90 deg and the dog can lie flat. U-slots, open at the bottom, slide down
// over the four servo gear towers (the round thighs hide them).
// Held on by 4 M2 x 10 screws through the sides into the deck blocks.
// The head screws onto the flat top (2 M2 x 10 + nuts, put in BEFORE the shell goes on).
// Holes: USB-C on the left side, OLED wires and 2 chin screws on top.
// Print open side DOWN with tree supports (Bambu Studio: Support on, type tree(auto)).
include <dog-puppy2-common.scad>
$fn=56;
tail_pts=[[-73,0,44],[-80,0,52],[-83,0,60],[-81,0,67],[-77,0,70]];
blk_x=[-13,13]; blk_z=deck_z+deck_t+5;          // from dog-deck-v3.scad
usb=[13,9]; usb_x=37.2; usb_z=46.6;            // XIAO USB-C (deck V3 echo)
module tail() for(i=[0:len(tail_pts)-2]) hull() { translate(tail_pts[i]) sphere(d=11-i*1.3); translate(tail_pts[i+1]) sphere(d=11-(i+1)*1.3); }
module gear_slot() for(sx=[-1,1]) hull() for(x=[shaft_x,shaft_x-gear_slot_back], z=[shaft_z,-10])
  translate([sx*x,0,z]) rotate([90,0,0]) cylinder(r=gear_slot_r,h=100,center=true);
module shell() {
  difference() {
    union() { shell_solid(0); tail(); }
    shell_solid(wall);
    gear_slot();
    for(x=blk_x) translate([x,0,blk_z]) rotate([90,0,0]) cylinder(d=m2_free,h=100,center=true);
    translate([usb_x-usb[0]/2,0,usb_z-usb[1]/2]) cube([usb[0],50,usb[1]]);
    translate([wire_hole[0][0],-wire_hole[1],shell_top-10]) cube([wire_hole[0][1]-wire_hole[0][0],2*wire_hole[1],20]);
    for(p=chin_screw) translate([p[0],p[1],shell_top-10]) cylinder(d=m2_free,h=20);
  }
}
shell();
assert(shell[1]/2+4<=leg_in_y,"Legs and thighs swing outside the body, 4mm+ gap");
assert(case_top_y<=shell[1]/2-wall-1,"Servo case inside the shell wall");
assert(gear_slot_r-6>=2,"Slot clears the 12mm gear tower by 2mm+");
assert(shaft_z+gear_slot_r+3<=deck_z,"Wall above each slot");
assert(usb_z-usb[1]/2>=shaft_z+15+2,"USB hole above the front thigh");
assert(abs(wire_hole[1])+1<abs(chin_screw[0][1])-m2_free/2,"Wire hole clear of the chin screws");
assert(max(abs(chin_screw[0][1]),wire_hole[1])<=shell[1]/2-shell_rp,"Chin screws and wire hole on the flat top");
echo(slot_x=[shaft_x-gear_slot_back-gear_slot_r,shaft_x+gear_slot_r], slot_top=shaft_z+gear_slot_r);
