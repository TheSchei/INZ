//2000 = 125mm
//1 = 62.5um


//Poprawki w proporcjach (odległości między gwintami na PCB)

scale = 1000;//85x125mm(wnętrze)
//d = 32;
d = 51.2;//3.2mm
HoleSize = 48;//3mm

scale(0.0625)//skalowanie
{
    //BOX();
    //color("green")
    //translate([scale*2, 0, 0])
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

module TrzymadelkaDoPB()
{
    //trzymadelka do powerbanku
       translate([-228-60+1,800,d/3])
            cube([64, 200, 200]);
            
       translate([-228-60+1,-816+198 - 32,d/3])
            cube([64, 200, 200]);
       translate([-680,-816+198 - 32,d/3])
            cube([450, 64, 8*16]);
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
    //rosuniecie wszystkich otworow od siebie o 2mm -> 32
    translate([-160 + 16, -950, 21 - HoleSize + 32 + 6.4])
        {
         translate([25+1.75*16 - 16,25+1.75*16 - 16,0])
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 2*64);
         translate([50*16 - 22 - 1.75*16 + 16,25+1.75*16 - 16,0])
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 2*64); 
         translate([25+1.75*16 - 16,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 2*64); 
         translate([50*16 - 22 - 1.75*16 + 16,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 2*64);        
        }    
}

module OtworyNaSruby()
{
    //rosuniecie wszystkich otworow od siebie o 2mm -> 32
    translate([-160 + 16, -950, 21 - HoleSize + 32])
        {
         translate([25+1.75*16 - 16,25+1.75*16 - 16,0])
        cylinder(r1 = HoleSize/2 + 10, r2 = HoleSize/2 + 10, h = 2*64);
         translate([50*16 - 22 - 1.75*16 + 16,25+1.75*16 - 16,0])
        cylinder(r1 = HoleSize/2 + 10, r2 = HoleSize/2 + 10, h = 2*64); 
         translate([25+1.75*16 - 16,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cylinder(r1 = HoleSize/2 + 10, r2 = HoleSize/2 + 10, h = 2*64); 
         translate([50*16 - 22 - 1.75*16 + 16,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cylinder(r1 = HoleSize/2 + 10, r2 = HoleSize/2 + 10, h = 2*64);        
        }    
}

module OtworyMniejsze()
{
    translate([-160 + 16, -950, 21 - HoleSize])
        {
         translate([25+1.75*16 - 16,25+1.75*16 - 16,0])
        cylinder(r1 = 18, r2 = 18, h = 2*64);
         translate([50*16 - 22 - 1.75*16 + 16,25+1.75*16 - 16,0])
        cylinder(r1 = 18, r2 = 18, h = 2*64); 
         translate([25+0.75*16,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cylinder(r1 = 18, r2 = 18, h = 2*64); 
         translate([50*16 - 22 - 1.75*16 + 16,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cylinder(r1 = 18, r2 = 18, h = 2*64);        
        }    
}

module PCBTower()
{
    //rzosuniecie wszystkich otworow od siebie o 2mm -> 32
    translate([-160 + 16, -950, 21 - HoleSize + 32 + 40 + 6.4])
        {
         translate([25+1.75*16 - 16,25+1.75*16 - 16 - 20,0])
        cube([96, 96 + 40, 2*64 - 48], true);
         translate([50*16 - 22 - 1.75*16 + 16 + 5,25+1.75*16 - 16 - 20,0])
        cube([96 + 10, 96 + 40, 2*64 - 48], true);
         translate([25+1.75*16 - 16,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cube([96, 96, 2*64 - 48], true); 
        translate([50*16 - 22 - 1.75*16 + 31,25 + 16 * 68-14 - 1.75*16 + 16,0])
        cube([96 + 15, 96, 2*64 - 48], true);
       }    
}

module OnOffHole()
{
    translate([scale/2 + 3*64, -scale*0.8, scale/4+40])
        cube([3*64, 6*16, 3*16], true);
    translate([scale/2 + 1.85*64, -scale*0.8, scale/4+40])
        cube([2.5*64, 10.5*16, 5.5*16], true);
}

module ConnectButtonHole()
{
    translate([650, 360, 64+6+129-12 + 30])
    rotate([0, 90, 0])
        cylinder(r1 = 80, r2 = 80, h = 2*64);
}

module SensorsHolderHoles()
{
    translate([50, 921 + 1.14*16, -d/4 + 101 - 80 + 24.8])
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 2*64);
    
    translate ([320, 797+74, -64/4 + 37 - 48 + 32]){
    translate([-24-47 + 4, 0, 0]) // zblizenie otworow o 0.5mm, odleglosc przed zmiana 5.92mm -> -4 dla obu
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 1.5*d);
    translate([24+47 - 4, 0, 0])
        cylinder(r1 = HoleSize/2, r2 = HoleSize/2, h = 1.5*d);}
}

module SensorsHoles()
{
translate([50, 750-12, 32]){
minkowski(){
        cube([240-160, 320-160, 100], true);
        sphere(80);
    }
    translate([0, 0, 7.566 + 64 - 40])
        cube([240, 320, 100], true);
}
    minkowski(){
    translate([320, 800, -20 + 32])
        cylinder($fn = 4, r2 = 10, r1 = 30, h = 60, center = true);
        sphere(20);
    }
}

module Boxxy()
{
    union(){
    translate([224 + 15 - 300 - 120 - 60, scale - 1.5*64, 0])
        cube([224 + 120+64, 1.5*64, 81 + 32]);//holder przy DHT
    
    Wregi();
        
    difference(){
    minkowski(){
    translate([0, 0, scale/4])
        cube([1.36*scale + 2*d/3, 2*scale + 2*d/3, scale/2], true);//pudło
    sphere(2*d/3);
    }
    translate([0, 0, scale*0.75 - 2.4*16 + 21])
        cube([2*scale, 4*scale, scale/2], true);//wygladzenie z gory
    
    translate([0, 0, scale/2 + d/3 + 32])
        cube([1.36*scale, 2*scale, scale], true);//pustka
    
    translate([-220-60-50, 300, 0])
        cube([100, 200, 50]);
    
    //korytko do wyciagania pb
    
    translate([-100, -1075, 440])
        cube([200, 50, 50]);
    
    //łapacz do pokrywki
}}
}

module Wregi()
{
    translate([-680,-1000,0])
        cube([32, 32, 483]);
    translate([680 - 32,1000 - 32,0])
        cube([32, 32, 483]);
    translate([680 - 32,-1000,0])
        cube([32, 32, 483]);
    translate([-680,1000 -32,0])
        cube([32, 32, 483]);
}

module WregiLID()
{
    translate([-680,-1000,0])
        cube([64, 128, 483]);
    translate([680 - 64,1000 - 128,0])
        cube([64, 128, 483]);
    translate([680 - 64,-1000,0])
        cube([64, 128, 483]);
    translate([-680,1000 - 128,0])
        cube([64, 128, 483]);
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
    translate([0, 0, 2*d - d/3])
        cube([2*scale, 4*scale , d], true);
    
    translate ([-scale/2 - 15, 0, 0]){
    translate ([0, 0, -d - 80])
        //OtworyNaPCB();
        OtworyNaSruby();
    translate ([0, 0, 0])
        OtworyMniejsze();
    }
    }
    difference()
    {
        translate([0, 0, 2*d - 10])
            magic();
        translate([0, 0, 2*d])
            cube([1.36*scale - 64, 2*scale - 64, 3*64], true);
        WregiLID();
    }
}
}

module magic()
{
    //cube([1.36*scale, 2*scale, d + 16], true);
    translate([0, -320, -2*d/3])
    rotate([0, 0, -45])
    cylinder(h = d+16 
     +2.4 + 6, r1 = scale - 32 - 7, r2 = scale - 16 - 32 - 7, $fn = 4);
    
    translate([0, 320, -2*d/3])
    rotate([0, 0, -45])
    cylinder(h = d+16 + 2.4 + 6, r1 = scale - 32 - 7, r2 = scale - 16 - 32 - 7, $fn = 4);
}




































