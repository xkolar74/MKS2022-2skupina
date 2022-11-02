close all;
clear all;

%% CSV read
raw = csvread("ntc.csv")';

temp1 = raw(1,:);
r = raw(2,:);

%% Calculate ADC values from given resistance
ad1 = r./(r+10).*(2^10);

figure;
plot(ad1, temp1, 'o');
xlabel('\itADC val\rm [arb. u.]');
ylabel('\itTemp\rm [deg. C]');
grid on;
hold on;

%% Interpolate the data
p = polyfit(ad1, temp1, 10);

ad2 = 0:1023;
temp2 = round(polyval(p, ad2), 1);

plot(ad2, temp2, 'r');

%% Generate output file
dlmwrite("data.dlm", temp2.*10, ',');



