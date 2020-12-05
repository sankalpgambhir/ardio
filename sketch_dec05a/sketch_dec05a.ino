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
                    text = Serial.readStringUntil('$');
                                           ki = text.toInt(); 
                                            if(ki == -7){                                                               
                                              analogWrite(led,255);          
                                                         }
                                            if(ki == 0){          
                                              analogWrite(led,0);          
                                              }
  }}
