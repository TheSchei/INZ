scale = 1000;
d = 10;
HoleSize = 40;

    BOX();
    color("green")
        translate([scale*1.5, 0, 0])
        //rotate([180, 0, 0])
        //translate([0, 0, -scale*0.55+5]) //<-zamkniecie pudelka ;)
            LID();


module BOX()
{
q = scale / 4;
union()
    {
       TrzymadelkaDoPB();
       TrzymadelkoDoDPDT();          
        
difference()
{
    minkowski(){
    translate([0, 0, scale/4])
        cube([scale, 2*scale, scale/2], true);//pudło
    sphere(d);
    }
    translate([0, 0, scale*0.75])
        cube([2*scale, 4*scale, scale/2], true);//wygladzenie z gory
    ///WYGLADZANIE MOZE PSUC!!!!
    
    
    translate([0, 0, scale/4 + d])
        cube([scale - 2*d, 2*scale - 2*d, scale/2], true);//pustka
    
    OtworyNaPCB();
    
    SensorsHolderHoles();
    SensorsHoles();
    
    ConnectButtonHole();
    OnOffHole();      
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
    
    
    translate([-400, -50, -d])    
        cylinder(r1 = HoleSize, r2 = HoleSize, h = 1.5*d);
    translate([-400, -50, -d])    
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 150);
    
    translate([-50, -50, -d])    
        cylinder(r1 = HoleSize, r2 = HoleSize, h = 1.5*d);
    translate([-50, -50, -d])    
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 150);
    
    translate([-50, -700, -d])    
        cylinder(r1 = HoleSize, r2 = HoleSize, h = 1.5*d);
    translate([-50, -700, -d])    
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 150);
    
    translate([-400, -700, -d])    
        cylinder(r1 = HoleSize, r2 = HoleSize, h = 1.5*d);
    translate([-400, -700, -d])    
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 150);
    
    
       
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

module uUSB(x)//stare
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

module standingHole(heigth)//stare
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

module TrzymadelkaDoPB()
{
    //trzymadelka do powerbanku
       translate([-50,50,0])
            cube([50, 200, 200]);
       translate([-50,550,0])
            cube([50, 200, 200]);
       //translate([-500,50,0])
            //cube([50, 200, 200]);
       //translate([-500,550,0])
            //cube([50, 200, 200]);
            
       translate([-500,-600,0])
            cube([50, 200, 200]);
       translate([-500,-600,0])
            cube([150, 50, 200]);
       translate([-50,-600,0])
            cube([50, 200, 200]);
       translate([-150,-600,0])
            cube([150, 50, 200]);
}

module TrzymadelkoDoDPDT()
{
    translate([350,25,0])
            cube([150, 200, 150]);
      translate([350,25,0])
            cube([70, 50, 250]);
      translate([350,175,0])
            cube([70, 50, 250]);
}

module OtworyNaPCB()
{
    translate([400,-700,0])
        cylinder(r1 = HoleSize/3, r2 = HoleSize/3, h = 1.5*d);
    translate([400,-50,0])
        cylinder(r1 = HoleSize/3, r2 = HoleSize/3, h = 1.5*d);
    translate([50,-50,0])
        cylinder(r1 = HoleSize/3, r2 = HoleSize/3, h = 1.5*d); 
    translate([50,-700,0])
        cylinder(r1 = HoleSize/3, r2 = HoleSize/3, h = 1.5*d);    
}

module pudlo()
{
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
}

module OnOffHole()
{
    translate([scale/2, -scale*0.8, scale/4])
        cube([3*d, 70, 35], true);
    translate([scale/2 - 1.5*d, -scale*0.8, scale/4])
        cube([3*d, 140, 70], true);
}

module ConnectButtonHole()
{
    translate([scale/2 - d - 1, scale/8, scale/4])
    rotate([0, 90, 0])
        cylinder(r1 = HoleSize, r2 = HoleSize, h = 2*d);
}

module SensorsHolderHoles()
{
translate([350, 900, -d/4])
        cylinder(r1 = HoleSize/3, r2 = HoleSize/3, h = 1.5*d);
    translate([250, 900, -d/4])
        cylinder(r1 = HoleSize/3, r2 = HoleSize/3, h = 1.5*d);
    translate([150, 900, -d/4])
        cylinder(r1 = HoleSize/3, r2 = HoleSize/3, h = 1.5*d);
}

module SensorsHoles()
{
minkowski(){ ////////<-NIESKALOWALNE
    translate([350, 750, 0])
        cube([20, 100, 100], true);
        sphere(40);
    }
    translate([350, 750, 50])
        cube([100, 180, 100], true);
    
    minkowski(){ ////////<-NIESKALOWALNE
    translate([200, 800, 0])
        cube([10, 10, 100], true);
        sphere(20);
    }
}