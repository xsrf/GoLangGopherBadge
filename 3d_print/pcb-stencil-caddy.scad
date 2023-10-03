difference() {
    translate([0,10,0]) cube([80,110,1],center=true);
    translate([0,-40,0]) cube([20,50,4],center=true);
    //translate([0,0,0]) cube([1,80,4],center=true);
    translate([-22.95,-96.05,-1]) linear_extrude(height=2) hull() import(file = "../pcb/pcb_outline.svg");
}





module round_edges($r = 0.1) {
    offset(r = -$r) 
        offset(r = 2*$r) 
        offset(r = -$r) 
        children();
}