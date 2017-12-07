long Now;
long last = 0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
Now = millis();
     if ( Now - last >= 2000 ){
     Serial.println("done");
     last = Now;
     }
}
