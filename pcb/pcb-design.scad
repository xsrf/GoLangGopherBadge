$fn=12;
$or = 0.2; // Outline rounding
layer="keepout";

//%scale([1,1,2])color("red") stroke_center(0.2) round_edges($or) import_fix("gopher01bw.svg","PCBOutline");

if(layer=="outline") {
    round_edges($or) import_fix("gopher01bw.svg","PCBOutline");
}

if(layer=="pawsplits") {
    import_fix("gopher01bw.svg","PawSplitsOutline");
}

if(layer=="silk") {
    difference(){
        import_fix("gopher01bw.svg","EyesContour");
        import_fix("gopher01bw.svg","Pupils");
    }
    offset(r=-0.15)import_fix("gopher01bw.svg","Tooth1");
    offset(r=-0.15)import_fix("gopher01bw.svg","Tooth2");
}

if(layer=="mask") {
    offset(0.4) import_fix("gopher01bw.svg","NoseBody");
    difference(){
        offset(r=-0.01) round_edges($or) import_fix("gopher01bw.svg","PCBOutline");
        //outline_wo_ears();
        import_fix("gopher01bw.svg","BodyContour");
        translate([0,30]) square([50,50],center=true);
        //import_fix("gopher01bw.svg","PawSplitsOutline");
    }
    import_fix("gopher01bw.svg","NoseTip");
    import_fix("gopher01bw.svg","Pupils");
    //stroke_out(0.4) import_fix("gopher01bw.svg","NoseBody");
    import_fix("gopher01bw.svg","InnerEars");
}

if(layer=="keepout") {
    import_fix("gopher01bw.svg","NoseBody");
    round_edges($or) difference() {
        offset(0.15) difference(){
            offset(-0.01) round_edges($or) outline_wo_ears();
            import_fix("gopher01bw.svg","BodyContour");
            //translate([0,30]) square([50,50],center=true);
            //offset(0.2) import_fix("gopher01bw.svg","InnerEars");
        }
        offset(0.05) import_fix("gopher01bw.svg","PawSplitsOutline");
    }
    offset(0.2) import_fix("gopher01bw.svg","Pupils");
    import_fix("gopher01bw.svg","InnerEars");
}



module outline_wo_ears() {
    difference() {
        import_fix("gopher01bw.svg","PCBOutline");
        translate([0,30]) square([50,50],center=true);
    }
    import_fix("gopher01bw.svg","BodyContour");
}



module import_fix($file,$layer) {
    scale(2.54) translate([-7,-147]) import($file,$layer);
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