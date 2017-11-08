#include <SoftwareSerial.h>
int x;
int y;
SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  pinMode(8, OUTPUT);
  Serial.begin(9600);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

}

void loop() { // run over and over
  if (Serial.available()) {
  //mySerial.write("x-axis: ");
  mySerial.write(Serial.read());  
  //mySerial.write("y-axis: ");
  //mySerial.write(Serial.read());  
/* if (mySerial.write(Serial.read()) == 101) {
 // x = Serial.read();
  mySerial.write("x-axis: ");
  mySerial.write(Serial.read());  
 }
 else{
  }
 if (mySerial.write(Serial.read()) == 102) {
  //y = Serial.read();
  mySerial.write(" y-axis: ");
  mySerial.write(Serial.read());  
 }
 else{
  }*/   
 }  
}

