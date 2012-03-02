clear all
close all



step = 2000000;
xmax = 2*10^8;


!./CREW_test1 > 1.data
data_elem = dlmread('./1.data','\t');
data_elem(:,1) = data_elem(:,1) - min(data_elem(:,1))+step;
data=data_elem;

for i=[2:50]    
    !./CREW_test1 > 1.data
    data_elem = dlmread('./1.data','\t');
    data_elem(:,1) = data_elem(:,1) - min(data_elem(:,1))+step;
    data = [data ; data_elem];
end


data1 = data(data(:,2)==1);
data2 = data(data(:,2)==2);

myBins = [min(data(:,1)):step:max(data(:,1))];
%myBins = [min(data(:,1)):step:xmax];

% Hists will be the same size because we set the bin locations:
%y1 = log(hist(data1(:,1), myBins));   
%y2 = log(hist(data2(:,1), myBins));
y1 = hist(data1(:,1), myBins);   
y2 = hist(data2(:,1), myBins);

% plot the results:

f = figure();
hold on
bar(myBins, [y1;y2]','stacked');
title('CREW\_test1');

xlabel('cpu cykler')
ylabel('antal læsninger/skrivninger')
set(f,'PaperType','B5');
axis([0 xmax 0 max(max(y1),max(y2))])
print (f,'-dpng','CREW_test1.png', '-r200');

hold off




!./CREW_test2 > 1.data
data_elem = dlmread('./1.data','\t');
data_elem(:,1) = data_elem(:,1) - min(data_elem(:,1))+step;
data=data_elem;

for i=[2:50]    
    !./CREW_test2 > 1.data
    data_elem = dlmread('./1.data','\t');
    data_elem(:,1) = data_elem(:,1) - min(data_elem(:,1))+step;
    data = [data ; data_elem];
end


data1 = data(data(:,2)==1);
data2 = data(data(:,2)==2);
myBins = [min(data(:,1)):step:max(data(:,1))];

% Hists will be the same size because we set the bin locations:
y1 = hist(data1(:,1), myBins);
y2 = hist(data2(:,1), myBins);
% plot the results:

f = figure();
hold on
bar(myBins, [y1;y2]','stacked');
title('CREW\_test2');

xlabel('cpu cykler')
ylabel('antal læsninger/skrivninger')
set(f,'PaperType','B5');
axis([0 xmax 0 450])
print (f,'-dpng','CREW_test2.png', '-r200');

hold off






!./PThread_rwlock > 1.data
data_elem = dlmread('./1.data','\t');
data_elem(:,1) = data_elem(:,1) - min(data_elem(:,1))+step;
data=data_elem;

for i=[2:50]    
    !./PThread_rwlock > 1.data
    data_elem = dlmread('./1.data','\t');
    data_elem(:,1) = data_elem(:,1) - min(data_elem(:,1))+step;
    data = [data ; data_elem];
end


data1 = data(data(:,2)==1);
data2 = data(data(:,2)==2);
myBins = [min(data(:,1)):step:max(data(:,1))];


% Hists will be the same size because we set the bin locations:
y1 = hist(data1(:,1), myBins);   
y2 = hist(data2(:,1), myBins);

% plot the results:


f = figure();
hold on
bar(myBins, [y1;y2]','stacked');
title('PThread\_rwlock');

xlabel('cpu cykler')
ylabel('antal læsninger/skrivninger')
set(f,'PaperType','B5');
axis([0 xmax 0 450])
print (f,'-dpng','PThread_rwlock.png', '-r200');

hold off

