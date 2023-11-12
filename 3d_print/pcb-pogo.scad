//#translate([18.9,-2.6,0]) cylinder(r=0.2,h=20);

scale([-1,1,1]) difference(){
    cube([50,70,20],center=true);
    translate([-22.95,-96.05,8]) linear_extrude(20) offset(0.35,$fn=64) import(file = "../pcb/pcb_outline.svg");
    translate([-22.95,-96.05,3]) linear_extrude(20) offset(-0.4) import(file = "../pcb/pcb_outline.svg");
    translate([0,-37,-12]) cylinder(r=12,h=30);
    translate([0,37,-12]) cylinder(r=12,h=30);
    translate([10,11.34,0]) #pogo(); // 3V3
    translate([10,8.8,0]) #pogo(); // SWIO
    translate([10,6.26,0]) #pogo(); // GND
    
    translate([7.1,7.3,0]) #pogo(); // DIN
    translate([-2.8,9.3,0]) #pogo(); // Vbatt
    
    translate([-15.4,9.8,0]) cylinder(r=5,h=20); // Switch
    //translate([15,9,0]) cube([6,6,10],center=true); // micro
    translate([-15.3,-6.5,10]) cube([6,6,20],center=true); // charger
    translate([0,17,10]) cube([35,6,26],center=true); // needle/pin
}

module pogo() {
    translate([0,0,-8]) cylinder(r=0.75,h=16,$fn=16);
    hull() {
        translate([0,0,-10.1]) cylinder(r=2,h=4,$fn=16);
        translate([0,0,-10.1]) cylinder(r=0.75,h=5,$fn=16);
    }
}

module round_edges($r = 0.1) {
    offset(r = -$r) 
        offset(r = 2*$r) 
        offset(r = -$r) 
        children();
}