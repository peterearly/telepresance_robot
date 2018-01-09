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
Servo myservo;
int8_t command;
int8_t x;
int8_t y;
int V;
int W;
int right_speed;
int left_speed;
char buf[3];
int Read;
int Read2;

void mot_drive_r(char mspeed, char dir)
{
  OCR0B = mspeed;
  if (dir == REVERSE) {
    digitalWrite(M1_DIR1, LOW);
    digitalWrite(M1_DIR2, HIGH);
  }

  if (dir == STOP) {
    digitalWrite(M1_DIR1, LOW);
    digitalWrite(M1_DIR2, LOW);
  }

  if (dir == FORWARD) {
    digitalWrite(M1_DIR1, HIGH);
    digitalWrite(M1_DIR2, LOW);
  }

}
void mot_drive_l(char mspeed, char dir)
{
  OCR0A = mspeed;
  if (dir == REVERSE) {
    digitalWrite(M2_DIR1, LOW);
    digitalWrite(M2_DIR2, HIGH);
  }
  if (dir == FORWARD) {
    digitalWrite(M2_DIR1, HIGH);
    digitalWrite(M2_DIR2, LOW);
  }
  if (dir == STOP) {
    digitalWrite(M2_DIR1, LOW);
    digitalWrite(M2_DIR2, LOW);
  }
}

void setup() {
  //SPEED CONTROL
  //WGM20 = PWM, phase correct  COM0A1 = PIN 5 COM0B1 = PIN 6
  TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);
  //turns on timer0 and sets pre scale to 0
  TCCR0B = (1 << CS00);


  pinMode(M1_PWR,  OUTPUT);
  pinMode(M1_DIR1, OUTPUT);
  pinMode(M1_DIR2, OUTPUT);
  pinMode(M2_PWR,  OUTPUT);
  pinMode(M2_DIR1, OUTPUT);
  pinMode(M2_DIR2, OUTPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  Read = digitalRead(10);
  Read2 = digitalRead(11);
  if (Serial.available()) {
    Serial.readBytes(buf, 3);
    command = buf[0];
    x = buf[1];
    y = buf[2];
  }
  
  if (command == 102) {
    myservo.attach(9);
    myservo.write(x);
  }

  if (command == 101) {
    V = (100 - abs(x)) * (y / 100) + y;
    W = (100 - abs(y)) * (x / 100) + x;
    right_speed = (V + W) / 2; //result for the speed of the right motor
    left_speed = (V - W) / 2; //result for the speed of the left motor

    if (right_speed > 0 and Read == LOW) {
      right_speed = map(right_speed, 0, 100, 150, TOP_SPEED);
      //Serial.println(right_speed);
      mot_drive_r(right_speed, FORWARD);
    }
    // motor goes reverse at a speed depending on the math above
   else if (Read2 == LOW) {
      right_speed = map(right_speed, 0, -100, 150, TOP_SPEED);
      //  Serial.println(right_speed);
      mot_drive_r(right_speed, REVERSE);
    }
    
    // motor goes forward at a speed depending on the math above
    if (left_speed > 0 and Read == LOW) {
      left_speed = map(left_speed, 0, 100, 150, TOP_SPEED);
      //Serial.println(left_speed);
      mot_drive_l(left_speed, FORWARD);

    }
    // motor goes reverse at a speed depending on the math above
   else if (Read2 == LOW) {
      left_speed = map(left_speed, 0, -100, 150, TOP_SPEED);
      //Serial.println(left_speed);
      mot_drive_l(left_speed, REVERSE);
    }
    
  }
}

