clc; clear; close all;
T = 0.1;
a = 1.5;
t_final = 20;
t_z = 0:T:t_final;              % tempo discreto
t_cont = 0:0.001:t_final;       % tempo contínuo
% ===============================
% CORES AJUSTADAS 
% ===============================
cor_zoh    = [0.4 0.8 0.4];  
cor_cont   = [0 0.8 0.8];     
cor_imp    = [1 1 0.4];     
cor_tustin = [1 1 1];       
% ===============================
% SISTEMA CONTÍNUO
% ===============================

% item 4
% num = [0 0 0 1];
% den = [1 0 0 0];
% num_inv = [T^2/2 T^2/2 0];
% den_inv = [1 -3 3 -1];

% item 8
% num = [0 0 a];
% den = [1 a 0];
% num_inv = [ (1 - exp(-a*T))  0 ];
% den_inv = [ 1  -(1 + exp(-a*T))  exp(-a*T) ];

% item 12
% num = [0 1 0];
% den = [1 0 a^2];
% num_inv = [1  -cos(a*T)  0];
% den_inv = [1  -2*cos(a*T)  1];

% item 11
 num = [0 0 a];
 den = [1 0 a^2];
 num_inv = [0  sin(a*T)  0];
 den_inv = [1  -(2*cos(a*T))  1];


sys_c = tf(num, den) % 1/s^3

% ===============================
% ENTRADAS
% ===============================
u_imp = [1 zeros(1,length(t_z)-1)];
u_deg = ones(size(t_z));
u_ra  = t_z;
% ===============================
% CONTÍNUO
% ===============================
[y_s_im, t_s_out_im] = impulse(sys_c, t_final);
[y_s_de, t_s_out_de] = step(sys_c, t_final);
y_s_ra = lsim(sys_c, t_cont, t_cont);
t_s_out_ra = t_cont;
% ===============================
% MÉTODOS DISCRETOS
% ===============================
sys_zoh = c2d(sys_c, T, 'zoh')
sys_tus = c2d(sys_c, T, 'tustin')
% Invariância ao impulso (manual)
sys_imp = tf(num_inv, den_inv, T)
% ===============================
% SIMULAÇÕES
% ===============================
% Impulso
y_z_imp   = lsim(sys_zoh, u_imp, t_z);
y_Z_im_2  = lsim(sys_imp, u_imp, t_z);
y_im_3    = lsim(sys_tus, u_imp, t_z);
% Degrau
y_z_de   = lsim(sys_zoh, u_deg, t_z);
y_Z_de_2 = lsim(sys_imp, u_deg, t_z);
y_de_3   = lsim(sys_tus, u_deg, t_z);
% Rampa
y_z_ra = lsim(sys_zoh, u_ra, t_z);
y_ra_2 = lsim(sys_imp, u_ra, t_z);
y_ra_3 = lsim(sys_tus, u_ra, t_z);
% ===============================
% ===== IMPULSO =====
% ===============================
figure; hold on
stem(t_z, y_z_imp, 'Color', cor_zoh, 'LineWidth', 2, 'Marker', 'none');
plot(t_s_out_im, y_s_im, 'Color', cor_cont, 'LineWidth', 2);
stem(t_z, y_Z_im_2, 'Color', cor_imp, 'LineWidth', 2,'Marker', 'none');
stem(t_z, y_im_3, 'Color', cor_tustin, 'LineWidth', 2,'Marker', 'none');
grid on;
title('Impulso: Discreto vs Contínuo');
xlabel('Tempo (s)');
ylabel('Amplitude');
legend('ZOH','Contínuo','Impulse Invariance','Tustin');
% ===============================
% ===== DEGRAU =====
% ===============================
figure; hold on
stem(t_z, y_z_de, 'Color', cor_zoh, 'LineWidth', 2, 'Marker', 'none');
plot(t_s_out_de, y_s_de, 'Color', cor_cont, 'LineWidth', 2);
stem(t_z, y_Z_de_2, 'Color', cor_imp, 'LineWidth', 2,'Marker', 'none');
stem(t_z, y_de_3, 'Color', cor_tustin, 'LineWidth', 2,'Marker', 'none');
grid on;
title('Degrau: Discreto vs Contínuo');
xlabel('Tempo (s)');
ylabel('Amplitude');
legend('ZOH','Contínuo','Impulse Invariance','Tustin');
% ===============================
% ===== RAMPA =====
% ===============================
figure; hold on
stem(t_z, y_z_ra, 'Color', cor_zoh, 'LineWidth', 2, 'Marker', 'none');
plot(t_s_out_ra, y_s_ra, 'Color', cor_cont, 'LineWidth', 2);
stem(t_z, y_ra_2, 'Color', cor_imp, 'LineWidth', 2,'Marker', 'none');
stem(t_z, y_ra_3, 'Color', cor_tustin, 'LineWidth', 2,'Marker', 'none');
grid on;
title('Rampa: Discreto vs Contínuo');
xlabel('Tempo (s)');
ylabel('Amplitude');
legend('ZOH','Contínuo','Impulse Invariance','Tustin');
% ===============================
% ===== IMPULSO =====
% ===============================
% ZOH vs Contínuo
figure; hold on
stem(t_z, y_z_imp, 'Color', cor_zoh, 'LineWidth', 2, 'Marker', 'none');
plot(t_s_out_im, y_s_im, 'Color', cor_cont, 'LineWidth', 2);
grid on;
title('Impulso: ZOH vs Contínuo');
xlabel('Tempo (s)');
ylabel('Amplitude');
legend('ZOH','Contínuo');
% Impulse Invariance vs Contínuo
figure; hold on
stem(t_z, y_Z_im_2, 'Color', cor_imp, 'LineWidth', 2,'Marker', 'none');
plot(t_s_out_im, y_s_im, 'Color', cor_cont, 'LineWidth', 2);
grid on;
title('Impulso: Impulse Invariance vs Contínuo');
xlabel('Tempo (s)');
ylabel('Amplitude');
legend('Impulse Invariance','Contínuo');
% Tustin vs Contínuo
figure; hold on
stem(t_z, y_im_3, 'Color', cor_tustin, 'LineWidth', 2,'Marker', 'none');
plot(t_s_out_im, y_s_im, 'Color', cor_cont, 'LineWidth', 2);
grid on;
title('Impulso: Tustin vs Contínuo');
xlabel('Tempo (s)');
ylabel('Amplitude');
legend('Tustin','Contínuo');
% ===============================
% ===== DEGRAU =====
% ===============================
% ZOH vs Contínuo
figure; hold on
stem(t_z, y_z_de, 'Color', cor_zoh, 'LineWidth', 2, 'Marker', 'none');
plot(t_s_out_de, y_s_de, 'Color', cor_cont, 'LineWidth', 2);
grid on;
title('Degrau: ZOH vs Contínuo');
xlabel('Tempo (s)');
ylabel('Amplitude');
legend('ZOH','Contínuo');
% Impulse Invariance vs Contínuo
figure; hold on
stem(t_z, y_Z_de_2, 'Color', cor_imp, 'LineWidth', 2,'Marker', 'none');
plot(t_s_out_de, y_s_de, 'Color', cor_cont, 'LineWidth', 2);
grid on;
title('Degrau: Impulse Invariance vs Contínuo');
xlabel('Tempo (s)');
ylabel('Amplitude');
legend('Impulse Invariance','Contínuo');
% Tustin vs Contínuo
figure; hold on
stem(t_z, y_de_3, 'Color', cor_tustin, 'LineWidth', 2,'Marker', 'none');
plot(t_s_out_de, y_s_de, 'Color', cor_cont, 'LineWidth', 2);
grid on;
title('Degrau: Tustin vs Contínuo');
xlabel('Tempo (s)');
ylabel('Amplitude');
legend('Tustin','Contínuo');
% ===============================
% ===== RAMPA =====
% ===============================
% ZOH vs Contínuo
figure; hold on
stem(t_z, y_z_ra, 'Color', cor_zoh, 'LineWidth', 2, 'Marker', 'none');
plot(t_s_out_ra, y_s_ra, 'Color', cor_cont, 'LineWidth', 2);
grid on;
title('Rampa: ZOH vs Contínuo');
xlabel('Tempo (s)');
ylabel('Amplitude');
legend('ZOH','Contínuo');
% Impulse Invariance vs Contínuo
figure; hold on
stem(t_z, y_ra_2, 'Color', cor_imp, 'LineWidth', 2,'Marker', 'none');
plot(t_s_out_ra, y_s_ra, 'Color', cor_cont, 'LineWidth', 2);
grid on;
title('Rampa: Impulse Invariance vs Contínuo');
xlabel('Tempo (s)');
ylabel('Amplitude');
legend('Impulse Invariance','Contínuo');
% Tustin vs Contínuo
figure; hold on
stem(t_z, y_ra_3, 'Color', cor_tustin, 'LineWidth', 2,'Marker', 'none');
plot(t_s_out_ra, y_s_ra, 'Color', cor_cont, 'LineWidth', 2);
grid on;
title('Rampa: Tustin vs Contínuo');
xlabel('Tempo (s)');
ylabel('Amplitude');
legend('Tustin','Contínuo');