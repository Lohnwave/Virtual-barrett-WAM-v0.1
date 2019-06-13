% Last revised: 2019-06-12
clear;
clc;
% close all;
L1 = Link('d', 0, 'a', 0, 'alpha', -pi/2,'qlim', [-2.6,2.6]);
L2 = Link('d', 0, 'a', 0, 'alpha', pi/2, 'qlim', [-2,2]);
L3 = Link('d', 0.55, 'a', 0.045, 'alpha', -pi/2,'qlim', [-2.8,2.8]);
L4 = Link('d', 0, 'a', -0.045, 'alpha', pi/2,'qlim', [-0.9,3.1]);
L5 = Link('d', 0.3, 'a', 0, 'alpha', -pi/2,'qlim', [-4.76,1.24]);
L6 = Link('d', 0, 'a', 0, 'alpha', pi/2,'qlim', [-1.6,1.6]);
L7= Link('d', 0.06, 'a', 0, 'alpha',0,'qlim', [-3,3]);
b=isrevolute(L1);  %Link 类函数
robot=SerialLink([L1,L2,L3,L4,L5,L6,L7]);   %SerialLink 类函数
robot.name='barrett wam 机器人';

J=[0 0 0 0 0 0 0];
theta=[0 0 0 0 0 0 0];
x0=500;
y0=0;
z0=200;
A0=pi/2;
B0=0;
C0=0;
%Limit move
L_Transl = 3.0;
L_Posture = 2.0;
% T1=transl(0.7632, 0, 0.3597);
% R1=[-0.7071 0 0.7071; 0 1 0; -0.7071 0 -0.7071]
tcpipServer=tcpip('localhost',3000,'NetworkRole','Server');
set(tcpipServer, 'InputBufferSize', 1024); 
set(tcpipServer, 'outputBufferSize', 1024);
set(tcpipServer,'Timeout',1);
fopen(tcpipServer);

tcpipUDP=udp('localhost','RemotePort', 8866, 'LocalPort', 8844);

point = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0];
recv = '0,0,0,0,0,0'
counter = 0;
while(1)
    try
%       recv= fread(tcpipServer,tcpipServer.BytesAvailable)';
        recv=fread(tcpipServer,60)';
        recv = char(recv);
        [~,strlen] = size(recv);
        begin = 1;
        index = 1;
        counter = counter+1;
        if strlen >1
            for i=1:strlen
                if(recv(i)==',')
                    a=recv(begin:i-1);
                    begin = i+1;
                    point(index) = str2double(a);
                    index = index + 1;
                end
            end
            %fprintf('%f, %f, %f, %f, %f, %f\n success \n',point(1), point(2), point(3),point(4), point(5), point(6));
%             x=x0-point(1)*1.5;
%             y=y0-point(2)*1.5;
%             z=z0+point(3)*1.5;
            x=x0-point(1);
            y=y0-point(2);
            z=z0+point(3);
            A=A0-point(4);
            B=B0+point(5);
            C=C0-point(6);
            T1=transl(x/1000, y/1000, z/1000);
%             R1=trotz(C*57.29)*troty(A*57.29)*trotx(B*57.29);
            R1=trotz(C)*troty(A)*trotx(B);
            T2=T1*R1;
            % J=ikine(robot,T2,J);
            J=ikine(robot,T2,theta);
%             robot.plot(J);
            % tranimate(R1);
            
            J1=num2str(J(1));
            J2=num2str(J(2));
            J3=num2str(J(3));
            J4=num2str(J(4));
            J5=num2str(J(5));
            J6=num2str(J(6));
            J7=num2str(J(7));
            % 防止求解角度过大 求解突变及防止运动过大
            if ((counter ~= 1)&& (sum(abs(J(1:4)-theta(1:4)))>L_Transl||sum(abs(J(5:7)-theta(5:7)))>L_Posture))
                J = theta;
                disp('  **error Angle is too large......');
            else
               message=[J1,',',J2,',',J3,',',J4,',',J5,',',J6,',',J7];
               fopen(tcpipUDP);
               fprintf(tcpipUDP,message);
               fprintf('%f, %f, %f, %f, %f, %f \n',J(1), J(2), J(3),J(4), J(5), J(6));
               fprintf('send success\n');
               fclose(tcpipUDP);
               theta = J;
            end
        end        
    catch
        disp('  **error leam motion lose......');
%         tcpipServer.ValuesReceived      
    end
end
fclose(tcpipServer);
delete(tcpipUDP);
clear(tcpipUDP);




