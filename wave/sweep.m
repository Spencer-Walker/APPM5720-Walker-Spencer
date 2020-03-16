close all
for i = 1:16
    clock_time(i) = load("clock_time"+i+".out");
end 
figure 
plot(clock_time)
title("cpu time vs threds")
xlabel("threads")
ylabel("time (\mu s)")

X = [ones(16,1) 1./(1:16)'];
y = clock_time'./clock_time(1);

b = (X'*X)\X'*y;
figure 
plot(1:16,y,1:16,X*b)
title('Least Squares Fit with Amdahls Law (normalized by T1)')
xlabel('threds')
ylabel('T/T_1')
legend('exact',"Fs="+b(1)+" Fp="+b(2))

for i = 1:16
    energy(:,i) = load("energy"+i+".out");
    error(i) = norm(energy(:,i)-energy(:,1));
end 
figure 
plot(error)
title("L2 error energy vs threds")
xlabel("threds")
ylabel("error")