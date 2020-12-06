function carr = getAudioAndSend2()
a = audiorecorder(1000,8,1);
recordblocking(a,2); %200 data points recorded 
d = getaudiodata(a,'int8');
%d = round(7.0*sin(0.6*(1:1:500)));
indices = find(d > 7);
d(indices) = 7;
indices = find(d < -7);
d(indices) = -7;
plot(d);

Ard = serial("COM3","BaudRate",115200);
fopen(Ard);
for i = 1:430
  fprintf(Ard,'%s\n',int2str(d(i)));
  pause(3/100);
end
pause(1);
for i=1:15
    y = fscanf(Ard,'%s');
    fprintf('%s\n', y);
end
fclose(Ard);
end