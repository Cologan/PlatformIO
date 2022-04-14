u = udpport;                                                    % Start UDP Port
writeline(u,"Matlab Connection","192.168.178.80",4210);         % Writes a String on given IP-Adress and UDP-Port
data = read(u,32);                                              % Reads from UDP Port and Size to be read
my_string = native2unicode(data,'ASCII');                       % Converts the recieved ASCII to String
disp(my_string);                                                % Display on Command window

%clear u;                                                        % Stops UDP 