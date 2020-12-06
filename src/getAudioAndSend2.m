function carr = getAudioAndSend2()
  a = audiorecorder(1000,8,1);
  recordblocking(a,2); %2000 data points recorded 

  d = getaudiodata(a,'int8');

  % clip the data at +- 7
  indices = find(d > 7);
  d(indices) = 7;
  indices = find(d < -7);
  d(indices) = -7;

  % show data
  plot(d);

  % connect to arduino
  Ard = serial("COM3","BaudRate",115200);
  fopen(Ard);

  % send 400 and change samples
  for i = 1:430
    fprintf(Ard,'%s\n',int2str(d(i)));
    pause(3/100); % prevents race condition!
  end
  
  pause(1);

  % read the output
  for i=1:15
      y = fscanf(Ard,'%s');
      fprintf('%s\n', y);
  end

  fclose(Ard);
end