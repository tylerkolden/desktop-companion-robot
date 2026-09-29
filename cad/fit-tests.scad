// Original parametric fit samples. Units: mm. Not structural robot parts.
// Export with: openscad -o servo-fit-gauge.stl -D 'part="servo"' fit-tests.scad
part = "servo"; // [servo,oled]
$fn = 48;

// WWZMDiB SG90, Amazon B0C7KPXPQV dimension image reviewed 2026-09-28.
// Body length derived from 7.9+14.7; width explicitly 12.5mm.
// Seller total 31.8 conflicts with 5+7.9+14.7+5=32.6; see SERVO-DIMENSIONS.md.
// Listing-derived fit sample, not a verified structural mount.
servo_length = 22.6;
servo_width = 12.5;
servo_clearances = [0.0, 0.4, 0.8]; // total added to aperture length and width
oled_board_width = 27.5;
oled_board_height = 27.8;
oled_clearance = 0.5;
wall = 2.0;

module rounded_plate(w,h,z,r=2) {
  linear_extrude(z) offset(r=r) offset(delta=-r) square([w,h]);
}

module servo_gauge() {
  // Three through-apertures: slide the servo body in only until its ears meet
  // the plate. The cable exits through a notch; no force or screws needed.
  difference() {
    rounded_plate(94, 28, 2.4);
    for(i=[0:2]) {
      gap = servo_clearances[i];
      translate([5+i*30,6,-0.1]) cube([servo_length+gap,servo_width+gap,2.6]);
      translate([5+i*30+servo_length/2-2,0,-0.1]) cube([4,7,2.6]);
      // One, two, three index dimples on the closed edge, readable without text fonts.
      for(j=[0:i]) translate([10+i*30+j*4,23,1.6]) cylinder(h=1,r=0.8);
    }
  }
}

module oled_gauge() {
  // Through-aperture checks board outline only: no lip bears on the glass.
  // This is not a retaining bracket; support the module with your hand.
  w=oled_board_width+oled_clearance;
  h=oled_board_height+oled_clearance;
  difference() {
    rounded_plate(w+2*wall,h+2*wall,2.4);
    translate([wall,wall,-0.1]) cube([w,h,3]);
    // Full-width header-edge opening, so installed pin headers aren't trapped.
    translate([wall+2,h+wall-1,-0.1]) cube([w-4,4,3]);
  }
}

assert(servo_length>0 && servo_width>0 && wall>=1);
assert(oled_board_width>0 && oled_board_height>0);
if(part=="servo") servo_gauge();
else if(part=="oled") oled_gauge();
else assert(false,"part must be servo or oled");
