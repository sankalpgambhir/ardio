
int SignalSize = 300;
int incomingString[300];
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(13,OUTPUT);
digitalWrite(13,0);
}

void loop() {
  // put your main code here, to run repeatedly:
int i =0; 
while(i< SignalSize){
  if (Serial.available() > 0) {
    incomingString[i] = Serial.read(); 
    
    i++;
  }
  
}
if(i>=100){Serial.write("N");
digitalWrite(13,1);
Serial.println(incomingString[200]);
 Serial.write(13);
  Serial.write(10);
}
}
