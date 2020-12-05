function carr = getAudioAndSend2()
a = audiorecorder(10000,8,1);
recordblocking(a,1); %200 data points recorded 
carr = getaudiodata(a,'int8')
Ard = serial("COM4","BaudRate",9600);
fopen(Ard);
for i = 1:200
  fprintf(Ard,'%d\n',int2str(carr(i+1500)));
end
fscanf(Ard)
fclose(Ard);
end