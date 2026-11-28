// Horn fit gauge: print this to find which hole the horn hub drops into and
// which slot the flat horn arm slides into. Measures what fits YOUR printer,
// which is exactly what the next leg needs. Units mm. OpenSCAD 2021.01.
$fn=64;
hubs=[7.0,7.5,8.0,8.5,9.0,9.5,10.0];   // hub holes, through
arms=[1.2,1.4,1.6,1.8,2.0,2.2,2.4];    // arm thickness slots, open at the front edge
pitch=11; plate=[85,36,3]; slot_depth=9; engrave=0.6;
module label(t) linear_extrude(engrave+.1) text(t,size=3,halign="center",valign="center",font="Liberation Sans:style=Bold");
difference() {
  cube(plate);
  for(i=[0:len(hubs)-1]) {
    x=9+i*pitch;
    translate([x,27,-.1]) cylinder(d=hubs[i],h=plate[2]+.2);
    translate([x,20.2,plate[2]-engrave]) label(str(hubs[i]));
    translate([x-arms[i]/2,-.1,-.1]) cube([arms[i],slot_depth+.1,plate[2]+.2]);
    translate([x,12.5,plate[2]-engrave]) label(str(arms[i]));
  }
}
