// Dog head V2 (Puppy V2): a small round head where the screen is most of the face.
// It sits straight on the flat top of the body (no neck, like a chubby puppy).
// part="shell": the round head with the big screen window and a little nose. Its lower edge
//   hugs the body shell.
// part="chin": the flat plate inside it. It carries the OLED slot and two posts for the ear
//   screws, and screws to the body roof.
// Assembly: chin onto the body roof with 2 M2 x 10 screws + nuts (nuts inside the body, so do
// this before the body shell goes on). OLED into its slot, wires down through the hole. Head
// shell over the chin, then the 2 ear screws (M2 x 10) through ear + head into the chin posts.
// Shell: print open side DOWN with tree supports. Chin: print flat, no supports.
include <dog-puppy2-common.scad>
part="shell"; // [shell,chin]
$fn=56;
hx0=head_cx-head[0]/2; hx1=head_cx+head[0]/2;
in_front=hx1-wall;
// OLED slot (board 27.5 x 27.8 from the owner fit gauge)
oled=[27.5,27.8]; oled_t=4.5; plate_t=2; rail_t=2; rail_cover=2.5;
br_d=plate_t+oled_t+2*clr+rail_t; br_w=oled[0]+2*clr+2*rail_t;
oled_bot=72;
br_top=oled_bot+oled[1]+clr+1;
win=[25,17]; win_z=oled_bot+oled[1]/2;
br_x1=in_front-0.3; br_x0=br_x1-br_d;
post=[8,7];
module head_shell() {
  difference() {
    union() {
      head_outer();
      translate([hx1-1,0,win_z-win[1]/2-5]) scale([0.55,1,0.75]) sphere(5);                // nose
    }
    head_solid(wall);
    translate([hx1-wall-1,-win[0]/2,win_z-win[1]/2]) cube([wall+3,win[0],win[1]]);       // screen window
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
      intersection() {
        for(sy=[-1,1]) translate([ear_x-post[0]/2, sy>0 ? 20 : -40, chin_top]) cube([post[0],20,ear_z+5-chin_top]);
        head_solid(wall+clr);
      }
    }
    translate([wire_hole[0][0],-wire_hole[1],chin_z-1]) cube([wire_hole[0][1]-wire_hole[0][0],2*wire_hole[1],chin_t+2]);  // OLED wires down into the body
    for(p=chin_screw) translate([p[0],p[1],chin_z-1]) cylinder(d=m2_free,h=chin_t+2);
    for(sy=[-1,1]) translate([ear_x,sy*head[1]/2,ear_z]) rotate([90,0,0]) cylinder(d=m2_pilot,h=60,center=true);
  }
}
if(part=="shell") head_shell(); else if(part=="chin") chin(); else assert(false,"part must be shell or chin");
assert(wire_hole[0][1]<=br_x0-1,"Wire hole behind the OLED slot");
assert(chin_screw[0][0]+m2_free/2+1<=ear_x-post[0]/2 || abs(chin_screw[0][1])+3<20,"Chin screws clear of the ear posts");
assert(chin_z>=shell_top,"Chin rests on the body roof");
assert(oled_bot>=face_z[0]+2 && br_top<=face_z[1]-1,"OLED slot faces the flat part of the face (height)");
assert(br_w/2<=face_w/2-face_r+sqrt(pow(face_r-wall,2)-pow(max(0,br_top-(face_z[1]-face_r)),2))-0.5,"OLED slot faces the flat part of the face (top corners)");
assert(win[0]<=oled[0]+2*clr-2,"Window narrower than the OLED board");
echo(head_x=[hx0,hx1], window_z=[win_z-win[1]/2,win_z+win[1]/2], oled_slot=[br_x0,br_x1,br_top], screen_share_of_face=win[0]*win[1]/(face_w*(face_z[1]-face_z[0])));
