clear;
clc;
L1 = Link('d', 0, 'a', 0, 'alpha', -pi/2,'qlim', [-2.6,2.6]);
L2 = Link('d', 0, 'a', 0, 'alpha', pi/2, 'qlim', [-2,2]);
L3 = Link('d', 0.55, 'a', 0.045, 'alpha', -pi/2,'qlim', [-2.8,2.8]);
L4 = Link('d', 0, 'a', -0.045, 'alpha', pi/2,'qlim', [-0.9,3.1]);
L5 = Link('d', 0.3, 'a', 0, 'alpha', -pi/2,'qlim', [-4.76,1.24]);
L6 = Link('d', 0, 'a', 0, 'alpha', pi/2,'qlim', [-1.6,1.6]);
L7= Link('d', 0.06, 'a', 0, 'alpha',0,'qlim', [-3,3]);
% L1 = Link('d', 0, 'a', 0, 'alpha', -pi/2);
% L2 = Link('d', 0, 'a', 0, 'alpha', pi/2);
% L3 = Link('d', 0.55, 'a', 0.045, 'alpha', -pi/2);
% L4 = Link('d', 0, 'a', -0.045, 'alpha', pi/2);
% L5 = Link('d', 0.3, 'a', 0, 'alpha', -pi/2);
% L6 = Link('d', 0, 'a', 0, 'alpha', pi/2);
% L7 = Link('d', 0.06, 'a', 0, 'alpha',0);
b=isrevolute(L1);  %Link 类函数
robot=SerialLink([L1,L2,L3,L4,L5,L6,L7]);   %SerialLink 类函数
%robot=SerialLink([L1,L2,L3,L4,L5,L6]);   %SerialLink 类函数
robot.name='barrett wam 机器人';
%teach(robot);
%T2=rotz(-108/180*pi)*roty(45/180*pi)*rotx(80/180*pi)
T2=rotx(0)
T=[0 0 0 0 0 0 0]
tcpipServer=tcpip('127.0.0.1',30000,'NetworkRole','Server');

for i=0:50
T1=transl(-0.5+i*0.005,-0.422+i*0.01,0.4);
T1(1:3,1:3)=T2
%T=robot.ikine(T1)
T=ikine(robot,T1,T)
T3=robot.fkine(T)
%hold on;
robot.plot(T);
end



