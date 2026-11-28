// Whole-dog picture for checking fit (not printed). Imports the STLs; regenerate them first.
include <dog-puppy-common.scad>
show_shell=true; leg_angle=0; fur=[0.94,0.76,0.48]; cream=[1,0.95,0.86]; brown=[0.48,0.29,0.14];
module S(f) import(str("stl/",f,".stl"));
color("lightgray") S("dog-body-v1");
color("khaki") translate([0,0,deck_z]) S("dog-deck-v2");
if(show_shell) { color(fur) S("dog-shell-v1"); color(fur) S("dog-head-v1-shell"); }
color("khaki") S("dog-head-v1-chin");
for(sy=[-1,1]) color(brown) mirror([0,sy<0?1:0,0]) translate([ear_x,43.5,ear_z]) rotate([90,0,0]) rotate([0,0,-8]) S("dog-ear-v1");
for(sx=[-1,1],sy=[-1,1]) color(cream) mirror([0,sy<0?1:0,0]) translate([sx*45,leg_out_y-0.5,15]) rotate([90,0,0]) rotate([0,0,leg_angle]) S("leg-v7");
for(sx=[-1,1],sy=[-1,1]) color("royalblue") translate([sx*39.4-11.5, sy>0? 24-22.7+4 : -24-4, 15-6.45]) cube([23,22.7,12.9]);
// boards (approximate blocks)
color("navy") translate([-58.5,-12.7,deck_top+3]) cube([62.5,25.4,1.6]);
color("black") translate([-50,5,deck_top+4.6]) cube([45,7.5,20]);
color("white") translate([51.7,-23.5,chin_z+chin_t]) cube([35,47,8.5]);
color("green") translate([60.3,2,chin_z+chin_t+11]) cube([17.8,21,1.6]);
color("midnightblue") translate([93,-13.75,86]) cube([4,27.5,27.8]);
color([0.4,0.9,1]) for(y=[-7,7]) translate([100.3,y,100]) rotate([0,90,0]) scale([1.2,1,1]) cylinder(d=7,h=0.3,$fn=30);
color("darkblue") translate([-10.5,-8,6]) cube([21,16,1.6]);
