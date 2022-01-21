% Oblicza współczynnik k na podstawie R1 i R2 w omach.
% Zakłada wartości:
% Q = 329.034
% Ro = 322000 
%
% Przykładowe wywołanie:
% >>kCalculator(394000,128500)
%
% ans =
%
%    0.0161
function k = kCalculator(R1, R2)
Ro = 322000;
Q = 329.034;


k = ((R1/Ro) + (R1/R2) + 1) / Q;
end

