clear 
clc

a = [pi/2 0 0];
b = [0 0 0];
R0 = rotz(a(1))*rotx(a(2))*roty(a(3));
R1 = rotz(b(1))*rotx(b(2))*roty(b(3));
% tranimate(R0);
trplot2(R0);