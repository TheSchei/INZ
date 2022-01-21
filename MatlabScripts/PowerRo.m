% Oblicza Ro na podstawie oporów, napięcia zasilania i zmierzonego napięcia
% na R2
R1 = 394000;
R2 = 98100;
Vcc = 3.72;
Vx = 0.597;
Ro = (Vx*R1*R2)/(R2*Vcc-Vx*R2-R1*Vx)/1000