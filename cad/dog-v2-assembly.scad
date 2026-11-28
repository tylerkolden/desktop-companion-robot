// Puppy V2 whole-dog picture for checking fit (not printed). Imports the STLs; regenerate them first.
// pose="stand": legs straight down. pose="lie": all four legs swung 90 deg forward, dog lying flat.
include <dog-puppy2-common.scad>
pose="stand"; show_shell=true; tail_angle=20;   // tail wag angle (side to side)
leg_angle = pose=="lie" ? 90 : 0;
lift = pose=="lie" ? 1 : 36;       // stand: paw bottom is 51 below the shaft (z=15); lie: on the thighs
fur=[0.94,0.76,0.48]; cream=[1,0.95,0.86]; brown=[0.48,0.29,0.14];
module S(f) import(str("stl/",f,".stl"));
translate([0,0,lift]) {
  color("lightgray") S("dog-body-v3");
  color("khaki") translate([0,0,deck_z]) S("dog-deck-v3");
  if(show_shell) { color(fur) S("dog-shell-v3"); color(fur) S("dog-head-v2-shell"); }
  color("khaki") S("dog-head-v2-chin");
  for(sy=[-1,1]) color(brown) mirror([0,sy<0?1:0,0]) translate([ear_x,head[1]/2+3,ear_z]) rotate([90,0,0]) rotate([0,0,-10]) S("dog-ear-v2");
  for(sx=[-1,1],sy=[-1,1]) color(cream) mirror([0,sy<0?1:0,0]) translate([sx*shaft_x,leg_out_y,shaft_z]) rotate([90,0,0]) rotate([0,0,leg_angle]) S("leg-v8");
  for(sx=[-1,1],sy=[-1,1]) color("royalblue") translate([sx*39.4-11.5, sy>0? 24-22.7+4 : -24-4, 15-6.45]) cube([23,22.7,12.9]);
  // wagging tail: tail part coordinates (x across, y up, z = thickness, pocket face at z=8.5) turned onto the tail shaft
  color(fur) translate([0,0,tail_shaft_z]) rotate([tail_angle,0,0]) translate([tail_face_x-8.5,0,0]) multmatrix([[0,0,1,0],[1,0,0,0],[0,1,0,0],[0,0,0,1]]) S("dog-tail-v1");
  color("royalblue") translate([tail_case_top_x,-6.1,tail_zc-11.25]) cube([22.7,12.2,22.5]);
  // boards (approximate blocks)
  color("navy") translate([-46,-12.7,deck_top+3]) cube([62.5,25.4,1.6]);
  color("black") translate([-40,5,deck_top+4.6]) cube([45,7.5,20]);
  color("white") translate([19.7,-23.5,deck_top]) cube([35,47,8.5]);
  color("green") translate([28.3,4,deck_top+11]) cube([17.8,21,1.6]);
  color("midnightblue") translate([66.2,-13.75,72]) cube([4,27.5,27.8]);
  color("darkblue") translate([-10.5,-8,6]) cube([21,16,1.6]);
  // screen "eyes" (drawn by the program later): two big round eyes
  color([0.4,0.9,1]) for(y=[-6,6]) translate([73.2,y,86.5]) rotate([0,90,0]) cylinder(d=9,h=0.3,$fn=36);
}
color([0.85,0.85,0.85,0.5]) translate([-120,-80,-0.5]) cube([240,160,0.5]);   // floor
