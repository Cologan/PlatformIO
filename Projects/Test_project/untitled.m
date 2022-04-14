u = udpport;                                                    %
writeline(u,"Matlab Connection","192.168.178.80",4210);
data = read(u,26);
my_string = native2unicode(data,'ASCII');
disp(my_string);