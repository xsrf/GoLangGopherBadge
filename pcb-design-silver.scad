$fn=12;
$or = 0.2; // Outline rounding
layer="outline";

//%scale([1,1,2])color("red") stroke_center(0.2) round_edges($or) import_fix("gopher01bw.svg","PCBOutline");

if(layer=="outline") {
    round_edges($or) import_fix("gopher01bw.svg","PCBOutline");
}

if(layer=="pawsplits") {
    import_fix("gopher01bw.svg","PawSplitsOutline");
}

if(layer=="silk") {
    difference(){
        offset(-0.15) import_fix("gopher01bw.svg","EyesContour");
        import_fix("gopher01bw.svg","Pupils");
    }
    difference() {
        union() {
            offset(r=-0.15)import_fix("gopher01bw.svg","Tooth1");
            offset(r=-0.15)import_fix("gopher01bw.svg","Tooth2");
        }
        offset(0.3) import_fix("gopher01bw.svg","NoseBody");
    }
}

if(layer=="mask") {
    offset(0.3) import_fix("gopher01bw.svg","NoseBody");
    difference(){
        round_edges($or) import_fix("gopher01bw.svg","PCBOutline");
        //outline_wo_ears();
        offset(-0.5) outline_w_ears();
        //translate([0,30]) square([50,50],center=true);
        //import_fix("gopher01bw.svg","PawSplitsOutline");
    }
    import_fix("gopher01bw.svg","NoseTip");
    //stroke_out(0.4) import_fix("gopher01bw.svg","NoseBody");
    offset(0.3) import_fix("gopher01bw.svg","InnerEars");
    stroke_center(0.3) import_fix("gopher01bw.svg","EyesContour");
    import_fix("gopher01bw.svg","Pupils");
    difference() {
        union() {
            offset(r=0.15)import_fix("gopher01bw.svg","Tooth1");
            offset(r=0.15)import_fix("gopher01bw.svg","Tooth2");
        }
        offset(r=-0.15)import_fix("gopher01bw.svg","Tooth1");
        offset(r=-0.15)import_fix("gopher01bw.svg","Tooth2");
    }
}

if(layer=="keepout") {
    import_fix("gopher01bw.svg","NoseBody");
    round_edges(0.1) difference() {
        offset(0.20) difference(){
            offset(-0.01) round_edges($or) outline_wo_ears();
            import_fix("gopher01bw.svg","BodyContour");
            //translate([0,30]) square([50,50],center=true);
            //offset(0.2) import_fix("gopher01bw.svg","InnerEars");
        }
        offset(0.05) import_fix("gopher01bw.svg","PawSplitsOutline");
    }
    offset(-0.3) import_fix("gopher01bw.svg","Pupils");
    import_fix("gopher01bw.svg","InnerEars");
}



module outline_wo_ears() {
    difference() {
        import_fix("gopher01bw.svg","PCBOutline");
        translate([0,30]) square([50,50],center=true);
    }
    import_fix("gopher01bw.svg","BodyContour");
}

module outline_w_ears() {
    difference() {
        import_fix("gopher01bw.svg","PCBOutline");
        translate([0,-20]) square([50,50],center=true);
    }
    import_fix("gopher01bw.svg","BodyContour");
}



module import_fix($file,$layer) {
    scale(3.6) translate([-7,-147]) import($file,$layer); // 4 first design
}


module round_edges($r = 0.1) {
    offset(r = -$r) 
        offset(r = 2*$r) 
        offset(r = -$r) 
        children();
}


module stroke_center($t = 1) {
    difference() {
        offset(r = $t/2) children();
        offset(r = -$t/2) children();
    }
}

module stroke_out($t = 1) {
    difference() {
        offset(r = $t) children();
        children();
    }
}