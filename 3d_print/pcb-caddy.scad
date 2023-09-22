difference() {
    translate([-2,-3,0]) cube([50,70,2]);
    translate([0,0,1]) linear_extrude(height=2) offset(0.35) import(file = "pcb_outline.svg", center = true);

    translate([0,0,-1]) difference() {
        linear_extrude(height=20) offset(-2) import(file = "pcb_outline.svg", center = true);
        translate([50,40,0])cube([100,5,100],center=true);
    }
}





module round_edges($r = 0.1) {
    offset(r = -$r) 
        offset(r = 2*$r) 
        offset(r = -$r) 
        children();
}