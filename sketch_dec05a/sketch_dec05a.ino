int led = 13;
String text;
int ki;
void setup() {
                //pinMode(led, OUTPUT);
                Serial.begin(9600);
                ki = 0;
}
void loop() { 
  if (Serial.available()){
                    text = Serial.readStringUntil('$'); //In this code we use $ as the terminator character 
                                           ki = text.toInt(); 
                                           if(ki ==0)digitalWrite(13,1);
                                           if(ki==-7)digitalWrite(13,0);
                                           
                                           
                                                   
                                              
  }}
