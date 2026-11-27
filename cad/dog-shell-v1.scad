// Dog shell V1 (puppy body): a round bean with a curly tail, lowered straight down over the deck.
// The neck pokes up through the hole in the roof. The shell rests on the neck shoulder and two
// rear posts, and the head sits on top of it and holds it down. Open belly.
// The legs and their round hip discs stay OUTSIDE the shell, like a toy dog's shoulders.
// Print open side DOWN with tree supports (Bambu Studio: Support on, type tree(auto)).
include <dog-puppy-common.scad>
$fn=56;
tail_pts=[[-66,0,50],[-74,0,58],[-78,0,68],[-76,0,77],[-71,0,81]];
module tail() for(i=[0:len(tail_pts)-2]) hull() { translate(tail_pts[i]) sphere(d=11-i*1.3); translate(tail_pts[i+1]) sphere(d=11-(i+1)*1.3); }
module shell() {
  difference() {
    union() { shell_solid(0); tail(); }
    shell_solid(wall);
    translate([neck_hi[0][0]-clr,-neck_hi[1]-clr,50]) cube([neck_hi[0][1]-neck_hi[0][0]+2*clr,2*(neck_hi[1]+clr),40]);
  }
}
shell();
assert(shell[1]/2<=leg_in_y-2,"Shell narrower than the legs");
assert(shell_bot>=servo_top+0.5,"Shell edge above the servo cases");
inner_y_at_deck=shell[1]/2-shell_R+sqrt(pow(shell_R-wall,2)-pow(shell_top-shell_R-deck_z,2));
assert(inner_y_at_deck>=24+2,"Deck fits inside the shell");
echo(inner_y_at_deck=inner_y_at_deck);
