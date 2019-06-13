clear;
clc;
L1 = Link('d', 0, 'a', 0, 'alpha', -pi/2,'qlim', [-2.6,2.6]);
L2 = Link('d', 0, 'a', 0, 'alpha', pi/2, 'qlim', [-2,2]);
L3 = Link('d', 0.55, 'a', 0.045, 'alpha', -pi/2,'qlim', [-2.8,2.8]);
L4 = Link('d', 0, 'a', -0.045, 'alpha', pi/2,'qlim', [-0.9,3.1]);
L5 = Link('d', 0.3, 'a', 0, 'alpha', -pi/2,'qlim', [-4.76,1.24]);
L6 = Link('d', 0, 'a', 0, 'alpha', pi/2,'qlim', [-1.6,1.6]);
L7= Link('d', 0.06, 'a', 0, 'alpha',0,'qlim', [-3,3]);
b=isrevolute(L1);  %Link �ຯ��
robot=SerialLink([L1,L2,L3,L4,L5,L6,L7]);   %SerialLink �ຯ��
robot.name='barrett wam';     %SerialLink ����ֵ
% robot.manuf='wave';     %SerialLink ����ֵ
robot.display();  %Link �ຯ��
theta=[0 0 0 0 0 0 0]; %��ʼλ�� �Ƕ�
robot.plot(theta);   %SerialLink �ຯ��
a = 0;
sign = 0;
while true
    try
        if sign == 0
            a = a+0.1;
        else
            a = a-0.1;
        end
        if a >= pi/2
            sign = 1;
        end
        if a < pi/4;
                sign = 0;
        end
        % --STEP 2-get position & pose------
        %  P = socket [x, y, z, Tx, Ty, Tz]
        P = [0,0,0.91, pi/2+a/10,0,0];
        % ---------------------------

        % --STEP 3-trans P ƽ��ѡ�����------
        % rotx roty rotz
        % P_m = trans(P);
        T1 = transl(P(1:3)); % ƽ��
        R = trotz(P(4))*trotx(P(5))*troty(P(6)); % ��ת

        T = T1*R
        % ----------------------------

        % --STEP 4-���˶�ѧ ��ùؽ���ת�Ƕ�------
        J = ikine(robot,T,theta)
 
        % ---------��ʼλ��------------
        % T0=robot.fkine(theta) % ���˶�ѧ ��ó�ʼλ�˾���
        
        % send(T)
        theta = J;
        robot.plot(J);
    catch
        fprintf('get error.........');
    end
end


% q2=robot.ikine(p1);  %���˶�ѧ
% j0=robot.jacob0(q2);    %�ſɱȾ���