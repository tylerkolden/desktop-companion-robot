// Dog head V1 (puppy). part="shell": the round head with the screen window, nose and USB hole.
//   Its lower edge is shaped to hug the body shell.
// part="chin": the flat plate inside it, on top of the neck. It carries the OLED slot, the mini
//   breadboard with the XIAO, and two posts for the ear screws.
// Assembly: chin onto the neck with 2 M2 x 10 screws, then the head shell over it, then the
// 2 ear screws (M2 x 10) through ear + head into the chin posts. The head also holds the body
// shell down.
// Shell: print open side DOWN with tree supports. Chin: print flat, no supports.
include <dog-puppy-common.scad>
part="shell"; // [shell,chin]
$fn=56;
hx0=head_cx-head[0]/2; hx1=head_cx+head[0]/2;
in_front=hx1-wall;
chin_top=chin_z+chin_t;
// OLED slot (board 27.5 x 27.8 from the owner fit gauge)
oled=[27.5,27.8]; oled_t=4.5; plate_t=2; rail_t=2; rail_cover=2.5;
br_d=plate_t+oled_t+2*clr+rail_t; br_w=oled[0]+2*clr+2*rail_t;
oled_bot=86;
br_top=oled_bot+oled[1]+clr+1;
win=[24,16]; win_z=oled_bot+oled[1]/2;
br_x1=in_front-0.3; br_x0=br_x1-br_d;
// mini breadboard (47 x 35) turned sideways; the XIAO sits at the left end, USB-C out the left side
bb=[35,47]; rim_t=1.2; rim_h=2; bb_x0=50;
usb=[13,9]; usb_z=chin_top+8.5+2.5+3.2; usb_x=bb_x0+rim_t+clr+bb[0]/2;
post=[8,7];
nhx=neck_hi[0]; nhy=neck_hi[1];
wire_hole=[[nhx[0]+neck_wall,nhx[1]-neck_wall],8];
screw_y=nhy-neck_wall/2;
module head_shell() {
  difference() {
    union() {
      head_outer();
      translate([hx1-1,0,win_z-win[1]/2-9]) scale([0.55,1,0.8]) sphere(6.5);              // nose
    }
    head_solid(wall);
    translate([hx1-wall-1,-win[0]/2,win_z-win[1]/2]) cube([wall+3,win[0],win[1]]);       // screen window
    translate([usb_x-usb[0]/2,20,usb_z-usb[1]/2]) cube([usb[0],40,usb[1]]);              // USB cable (left side)
    for(sy=[-1,1]) translate([ear_x,sy*(head[1]/2),ear_z]) rotate([90,0,0]) cylinder(d=m2_free,h=16,center=true);
  }
}
module chin_plate() intersection() { head_solid(wall+clr); translate([hx0-1,-head[1],chin_z]) cube([head[0]+2,2*head[1],chin_t]); }
module chin() {
  difference() {
    union() {
      chin_plate();
      intersection() { head_solid(wall+0.2); translate([br_x0,-br_w/2,chin_top]) difference() {          // OLED slot; window lines up with the head
        cube([br_d,br_w,br_top-chin_top]);
        translate([rail_t,rail_t,oled_bot-chin_top]) cube([oled_t+2*clr,oled[0]+2*clr,60]);
        translate([-1,rail_t+rail_cover,oled_bot-chin_top]) cube([rail_t+2,oled[0]+2*clr-2*rail_cover,60]);
        translate([br_d-plate_t-1,br_w/2-win[0]/2,win_z-win[1]/2-chin_top]) cube([plate_t+2,win[0],win[1]]);
      } }
      translate([bb_x0,-bb[1]/2-clr-rim_t,chin_top]) difference() {   // breadboard rim, open on the USB side
        cube([bb[0]+2*clr+2*rim_t,bb[1]+2*clr+rim_t,rim_h]);
        translate([rim_t,rim_t,-1]) cube([bb[0]+2*clr,bb[1]+2*clr+1,rim_h+2]);
      }
      intersection() {
        for(sy=[-1,1]) translate([ear_x-post[0]/2, sy>0 ? 20 : -40, chin_top]) cube([post[0],20,ear_z+5-chin_top]);
        head_solid(wall+clr);
      }
    }
    translate([wire_hole[0][0],-wire_hole[1],chin_z-1]) cube([wire_hole[0][1]-wire_hole[0][0],2*wire_hole[1],chin_t+2]);  // wires up from the neck
    for(sy=[-1,1]) translate([(nhx[0]+nhx[1])/2,sy*screw_y,chin_z-1]) cylinder(d=m2_free,h=chin_t+2);
    for(sy=[-1,1]) translate([ear_x,sy*head[1]/2,ear_z]) rotate([90,0,0]) cylinder(d=m2_pilot,h=60,center=true);
  }
}
if(part=="shell") head_shell(); else if(part=="chin") chin(); else assert(false,"part must be shell or chin");
assert(bb_x0+bb[0]+2*clr+2*rim_t<=br_x0-0.5,"Breadboard clear of the OLED slot");
assert(bb_x0>nhx[1],"Breadboard in front of the neck wire hole");
assert(wire_hole[1]+0.5<=screw_y-m2_free/2,"Wire hole clear of the chin screws");
assert(chin_z>=shell_top+1,"Chin above the body shell roof");
assert(abs(usb_x-ear_x)>=usb[0]/2+13,"USB hole clear of the ear");
assert(oled_bot>=face_z[0]+2 && br_top<=face_z[1]-1,"OLED slot faces the flat part of the face (height)");
assert(br_w/2<=face_w/2-face_r+sqrt(pow(face_r-wall,2)-pow(max(0,br_top-(face_z[1]-face_r)),2))-0.5,"OLED slot faces the flat part of the face (top corners)");
echo(head_x=[hx0,hx1], window_z=[win_z-win[1]/2,win_z+win[1]/2], oled_slot=[br_x0,br_x1,br_top], usb=[usb_x,usb_z]);
