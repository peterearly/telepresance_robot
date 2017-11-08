#include <SoftwareSerial.h>
#include <Wire.h>
#include <Servo.h>
#include <avr/io.h> // includes external libraries
#include <avr/interrupt.h>

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

int x;
int y;
int V;
int W;
int right_speed;
int left_speed;
SoftwareSerial mySerial(10, 11); // RX, TX

void mot_drive_r(char mspeed,char dir)
{
  OCR0B = mspeed;
  if (dir == REVERSE){    
    digitalWrite(M1_DIR1, LOW);
    digitalWrite(M1_DIR2, HIGH);
  }

  if (dir == STOP){
    digitalWrite(M1_DIR1, LOW);
    digitalWrite(M1_DIR2, LOW);
  }

  if (dir == FORWARD){
    digitalWrite(M1_DIR1, HIGH);
    digitalWrite(M1_DIR2, LOW);
  }

}
void mot_drive_l(char mspeed, char dir)
{
  OCR0A = mspeed;
  if (dir == REVERSE){
    digitalWrite(M2_DIR1, LOW);
    digitalWrite(M2_DIR2, HIGH);
  }
  if (dir == FORWARD){  
    digitalWrite(M2_DIR1, HIGH);
    digitalWrite(M2_DIR2, LOW);
  }
  if (dir == STOP){
    digitalWrite(M2_DIR1, LOW);
    digitalWrite(M2_DIR2, LOW);
  }
}

void setup() {
  //SPEED CONTROL
  //WGM20 = PWM, phase correct  COM0A1 = PIN 5 COM0B1 = PIN 6
  TCCR0A = (1 << COM0A1) |(1 << COM0B1) |(1 << WGM00);
  //turns on timer0 and sets pre scale to 0
  TCCR0B = (1 << CS00);

  pinMode(IPT_PIN,  INPUT);  
  pinMode(IPT_PIN2, INPUT);  
  pinMode(M1_PWR,  OUTPUT);
  pinMode(M1_DIR1, OUTPUT);
  pinMode(M1_DIR2, OUTPUT);
  pinMode(M2_PWR,  OUTPUT);
  pinMode(M2_DIR1, OUTPUT);
  pinMode(M2_DIR2, OUTPUT);

  // Open serial communications and wait for port to open:
  pinMode(8, OUTPUT);
  Serial.begin(9600);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

}

void loop() { // run over and over
  if (Serial.available()) {
 
 Serial.write(Serial.read());  
 x = Serial.write(Serial.read());
 Serial.write(Serial.read());  
 y = Serial.write(Serial.read());
  
  V = (100 - abs(x)) * (y/100) + y;
  W = (100 - abs(y)) * (x/100) + x;
  right_speed = (V+W) /2; //result for the speed of the right motor
  left_speed = (V-W) /2; //result for the speed of the left motor
  
  if(right_speed > 0){
        right_speed = map(right_speed, 0, 100, 150, TOP_SPEED);
         //Serial.println(right_speed);
        mot_drive_r(right_speed, FORWARD);
      }
      // motor goes reverse at a speed depending on the math above
      else{
        right_speed = map(right_speed, 0, -100, 150, TOP_SPEED);
      //  Serial.println(right_speed);
        mot_drive_r(right_speed, REVERSE);
      }
      // motor goes forward at a speed depending on the math above      
      if(left_speed > 0){
        left_speed = map(left_speed, 0, 100, 150, TOP_SPEED);
        //Serial.println(left_speed);
        mot_drive_l(left_speed, FORWARD);
       
      }
      // motor goes reverse at a speed depending on the math above      
      else{
        left_speed = map(left_speed, 0, -100, 150, TOP_SPEED);
        //Serial.println(left_speed);
        mot_drive_l(left_speed, REVERSE);
   }   
 }  
}

