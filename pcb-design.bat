openscad -D "layer=\"silk\"" -o pcb_silk.svg pcb-design.scad
openscad -D "layer=\"mask\"" -o pcb_mask.svg pcb-design.scad
openscad -D "layer=\"outline\"" -o pcb_outline.svg pcb-design.scad
openscad -D "layer=\"keepout\"" -o pcb_keepout.svg pcb-design.scad