//2000 = 125mm
//1 = 62.5um


//Poprawki w proporcjach (odległości między gwintami na PCB)

scale = 1000;//85x125mm(wnętrze)
d = 64;//4mm
HoleSize = 48;//3mm

//scale(0.0625)//skalowanie
{
    BOX();
    color("green")
    translate([scale*2, 0, 0])
        //rotate([0, 180, 0])
        //translate([0, 0, -scale*0.55-5]) //<-zamkniecie pudelka ;)
            LID();
}

module BOX()
{
union()
{
    TrzymadelkaDoPB();
    TrzymadelkoDoDPDT();
difference()
{
    union(){
        Boxxy();
        PCBTower();
    }
    OtworyNaPCB();
    SensorsHolderHoles();
    SensorsHoles();
    
    ConnectButtonHole();
    OnOffHole();
}}}

module Hole(heigth)
{
    cylinder(d=HoleSize, h=heigth, $fn=6);//otwory!!!
}

module TrzymadelkaDoPB()
{
    //trzymadelka do powerbanku
       translate([-228-60+1,800,d/3])
            cube([64, 200, 200]);
            
       translate([-228-60+1,-816+198,d/3])
            cube([64, 200, 200]);
       translate([-728,-816+198,d/3])
            cube([500, 64, 8*16]);
}

module TrzymadelkoDoDPDT()
{
    translate([13.6+62, 215, 0]){
    translate([430,25,0])
        cube([70, 240, 150]);
    translate([430,25,0])
        cube([70, 47.5+12, 250]);
    translate([430,217.5-12,0])
        cube([70, 47.5+12, 250]);}
}

module OtworyNaPCB()
{
    //rzosuniecie wszystkich otworow od siebie o 2mm -> 32
    translate([-160 + 16, -950, 21 - HoleSize])
        {
         translate([25+1.75*16 - 16,25+1.75*16 - 16,0])
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 2*d);
         translate([50*16 - 22 - 1.75*16 + 16,25+1.75*16 - 16,0])
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 2*d); 
         translate([25+1.75*16 - 16,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 2*d); 
         translate([50*16 - 22 - 1.75*16 + 16,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 2*d);        
        }    
}

module OtworyMniejsze()
{
    translate([-160 + 16, -950, 21 - HoleSize])
        {
         translate([25+1.75*16 - 16,25+1.75*16 - 16,0])
        cylinder(r1 = HoleSize/4, r2 = HoleSize/4, h = 2*d);
         translate([50*16 - 22 - 1.75*16 + 16,25+1.75*16 - 16,0])
        cylinder(r1 = HoleSize/4, r2 = HoleSize/4, h = 2*d); 
         translate([25+0.75*16,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cylinder(r1 = HoleSize/4, r2 = HoleSize/4, h = 2*d); 
         translate([50*16 - 22 - 1.75*16 + 16,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cylinder(r1 = HoleSize/4, r2 = HoleSize/4, h = 2*d);        
        }    
}

module PCBTower()
{
    //rzosuniecie wszystkich otworow od siebie o 2mm -> 32
    translate([-160 + 16, -950, 21 - HoleSize])
        {
         translate([25+1.75*16 - 16,25+1.75*16 - 16,0])
        cylinder(r1 = HoleSize, r2 = HoleSize, h = 2*d - 48);
         translate([50*16 - 22 - 1.75*16 + 16,25+1.75*16 - 16,0])
        cylinder(r1 = HoleSize, r2 = HoleSize, h = 2*d - 48); 
         translate([25+1.75*16 - 16,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cylinder(r1 = HoleSize, r2 = HoleSize, h = 2*d - 48); 
         translate([50*16 - 22 - 1.75*16 + 16,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cylinder(r1 = HoleSize, r2 = HoleSize, h = 2*d - 48);        
        }    
}

module OnOffHole()
{
    translate([scale/2 + 3*d, -scale*0.8, scale/4+40])
        cube([3*d, 6*16, 3*16], true);
    translate([scale/2 + 2*d, -scale*0.8, scale/4+40])
        cube([2.5*d, 10.5*16, 5.5*16], true);
}

module ConnectButtonHole()
{
    translate([650, 360, d+6+129-12])
    rotate([0, 90, 0])
        cylinder(r1 = 80, r2 = 80, h = 2*d);
}

module SensorsHolderHoles()
{
    translate([50, 921 + 1.14*16, -d/4 + 101 - 80 - 4])
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 2*d);
    
    translate ([320, 797+74, -d/4 + 37 - 48]){
    translate([-24-47 + 4, 0, 0]) // zblizenie otworow o 0.5mm, odleglosc przed zmiana 5.92mm -> -4 dla obu
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 1.5*d);
    translate([24+47 - 4, 0, 0])
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 1.5*d);}
}

module SensorsHoles()
{
translate([50, 750-12, 0]){
minkowski(){
        cube([240-160, 320-160, 100], true);
        sphere(80);
    }
    translate([0, 0, 7.566 + d - 40])
        cube([240, 320, 100], true);
}
    minkowski(){
    translate([320, 800, -20])
        cylinder($fn = 4, r2 = 10, r1 = 30, h = 50, center = true);
        sphere(20);
    }
}

module Boxxy()
{
    union(){
    translate([224 + 15 - 300 - 120, scale - 1.5*d, 0])
        cube([224 + 120, 1.5*d, 81]);
    difference(){
    minkowski(){
    translate([0, 0, scale/4])
        cube([1.36*scale + 2*d/3, 2*scale + 2*d/3, scale/2], true);//pudło
    sphere(2*d/3);
    }
    translate([0, 0, scale*0.75 - 2.4*16])
        cube([2*scale, 4*scale, scale/2], true);//wygladzenie z gory
    
    translate([0, 0, scale/2 + d/3])
        cube([1.36*scale, 2*scale, scale], true);//pustka
    
    
    translate([-220-60, 450, 40])
    rotate([90, 0, 0])
        cylinder (r1 = 50, r2 = 50, h = 200);
    
    //korytko do wyciagania pb
    
    translate([-100, -1070, 450])
    rotate([90, 0, 90])
        cylinder (r1 = 50, r2 = 50, h = 200);
    
    //łapacz do pokrywki
}}
}

module LID()
{
union()
{
    difference(){
    minkowski(){
    translate([0, 0, d])
        cube([1.36*scale + 2*d/3, 2*scale + 2*d/3, 2*d/3], true);
        sphere(2*d/3);
    }
    translate([0, 0, 2*d - d/3 + 10])
        cube([2*scale, 4*scale , d], true);
    
    translate ([-scale/2 - 15, 0, 0]){
    translate ([0, 0, -d + 32])
        OtworyNaPCB();
    translate ([0, 0, 0])
        OtworyMniejsze();
    }
    }
    difference()
    {
        translate([0, 0, 2*d-2])
            magic();
        translate([0, 0, 2*d])
            cube([1.36*scale - d, 2*scale - d, 3*d], true);
    }
}
}

module magic()
{
    //cube([1.36*scale, 2*scale, d + 16], true);
    translate([0, -320, -2*d/3])
    rotate([0, 0, -45])
    cylinder(h = d+16 
     +2.4, r1 = scale - 32 - 8+1, r2 = scale - 16 - 32 - 8+1, $fn = 4);
    
    translate([0, 320, -2*d/3])
    rotate([0, 0, -45])
    cylinder(h = d+16 + 2.4, r1 = scale - 32 - 8+1, r2 = scale - 16 - 32 - 8+1, $fn = 4);
}




































