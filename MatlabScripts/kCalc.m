%%Oporniki w dzielniku w omach
R1 = 394000;
R2 = 128500;
%%Stałe
Ro = 322000;
Q = 329.034;

k = ((R1/Ro) + (R1/R2) + 1) / Q;
disp(sprintf('k = %f', k));