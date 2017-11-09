#include <SoftwareSerial.h>


#define IPT_PIN    2       //physical pin 4 INPUT PIN encoder
#define IPT_PIN2   3      //physical pin 5 INPUT PIN encoder
#define M1_PWR     5        //physical pin 11
#define M2_PWR     6        //physical pin 12
#define M1_DIR1    7       //physical pin 13
#define M1_DIR2    8       //physical pin 14
#define M2_DIR1    12      //physical pin 18
#define M2_DIR2    13      //physical pin 19


#define FORWARD  1
#define REVERSE  2
#define STOP     3

#define TOP_SPEED 235
String a;
int8_t x;
int8_t y;
//int z;
int V;
int W;
int command;
int right_speed;
int left_speed;
SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  
  // Open serial communications and wait for port to open:
//  pinMode(8, OUTPUT);
  Serial.begin(9600);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

}

void loop() { // run over and over
  //mySerial.write(101);
  if (Serial.available()) {
 // command = Serial.read();
 // if (command == 101){
 //a = Serial.readString();
 // x = Serial.read();
 // y = Serial.read();
  mySerial.write(Serial.read());
//  Serial.print(',');
 // Serial.println(y);
 // }

  //V = (100 - abs(x)) * (y/100) + y;
 // W = (100 - abs(y)) * (x/100) + x;
 // right_speed = (V+W) /2; //result for the speed of the right motor
 // left_speed = (V-W) /2; //result for the speed of the left motor
 // Serial.println(right_speed);
//  Serial.println(right_speed);
//  Serial.println(left_speed);
  
 }  
}

