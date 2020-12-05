
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
  if (Serial.available() > 0) { //Whenever a byte is available, read it and add it to the array
    incomingString[i] = Serial.read(); 
    i++;
  }
  
}
if(i>=SignalSize) //Check whether all expected Bytes are here
{Serial.write("N"); 
digitalWrite(13,1); //Glow LED for confirmation 
Serial.println(incomingString[200]); //Send Confirmation bacl to MALAB
 Serial.write(13); //Next two lines are terminator
  Serial.write(10);
}
}
