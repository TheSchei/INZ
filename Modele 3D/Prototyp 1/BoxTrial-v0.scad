scale = 1000;
d = 30;
HoleSize = 40;

    BOX();
    color("green")
        translate([scale*1.5, 0, 0])
        //rotate([180, 0, 0])
        //translate([0, 0, -scale*0.55+5]) //<-zamkniecie pudelka ;)
            LID();


module BOX()
{
union()
    {
      q = scale / 4;
      difference(){
      translate([0, -scale*0.75, q/2])
          cube([scale, scale/2, q], true); //Półka na czujniki
          
      translate([250,-900,0])
          Hole(scale); ////////<-NIESKALOWALNE
          
      }
      difference()
        {
            translate([-scale/20, -scale, scale/4])
                cube([scale/10, scale/2, scale/4]);//ścianka między czujnikami
            
            translate([-scale/10, -scale, scale/2 - d])
                cube([scale/5, 2*d, scale/2]);//miejsce na to coś wystające z pokrywki
        }
        
        translate([400,-350,0])
            standingHole(200); ////////<-NIESKALOWALNE
        translate([400,-50,0])
            standingHole(200); ////////<-NIESKALOWALNE
        translate([-400,-50,0])
            standingHole(200); ////////<-NIESKALOWALNE
        translate([-400,-350,0])
            standingHole(200); ////////<-NIESKALOWALNE
        
        
        
        
difference()
{
    ///
    minkowski(){
    translate([0, 0, scale/4])
        cube([scale, 2*scale, scale/2], true);//pudło
    sphere(20);
    }
    translate([0, 0, scale*0.75])
        cube([2*scale, 4*scale, scale/2], true);//wygladzenie z gory
    ///WYGLADZANIE MOZE PSUC!!!!
    
    
    
    translate([0, 0, scale/4 + d])
        cube([scale - 2*d, 2*scale - 2*d, scale/2], true);//pustka
    translate([scale/2, -scale/5, scale/4])
        //rotate([180, 0, 0])
        uUSB(100);//USB hole ////////<-NIESKALOWALNE
    
    translate([scale/2, scale/8, scale/4])
        cube([3*d, 120, 70], true);//POWER switch hole? ////////<-NIESKALOWALNE
        
}
}
}

module LID()
{
union()
{
    difference(){
    minkowski(){
    translate([0, 0, d])
        cube([scale, 2*scale, d], true);
        sphere(20);
    }
    translate([0, 0, 2*d])
        cube([2*scale, 4*scale, d], true);
    
    
    
    minkowski(){ ////////<-NIESKALOWALNE
    translate([250, -700, 0])
        cube([20, 150, 100], true);
        sphere(80);
    }
    
    
    
    }
    difference()
    {
        translate([0, 0, 2*d])
            cube([scale - 2*d, 2*scale - 2*d, 2*d], true);
        translate([0, 0, 2*d])
            cube([scale - 4*d, 2*scale - 4*d, 3*d], true);
    }
}
}

module uUSB(x)
{
    difference()
    {
            cube([x, x, x/3], true);
            translate([0, x, -x/5])
               rotate([45, 0, 0])
                cube([3*x, x, x], true);
            translate([0, -x, -x/5])
               rotate([45, 0, 0])
                cube([3*x, x, x], true);
    }
}

module standingHole(heigth)
{
    difference(){
        cylinder(r1 = HoleSize, r2 = HoleSize, h = heigth);
        cylinder(d=HoleSize, h=heigth + 1, $fn=6);
    }
}

module Hole(heigth)
{
    cylinder(d=HoleSize, h=heigth, $fn=6);//otwory!!!
}





