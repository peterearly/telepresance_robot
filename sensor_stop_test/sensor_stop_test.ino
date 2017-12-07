#include <SoftwareSerial.h>
#include <Wire.h>
#include <Servo.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Adafruit_VL53L0X.h>
#define TCAADDR 0x70

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

int8_t command;
int8_t x;
int8_t y;
int V;
int W;
int right_speed;
int left_speed;
char buf[3];
//int STOP;
int GO;

//long Now;
//long last = 0;
//long go;
//long last2 = 0;
//int sensor = 0;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Servo myservo;

void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

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
  Wire.begin();
  Serial.begin(9600);

  //while (! Serial) {
  // delay(1);
  //}
  pinMode(M1_PWR,  OUTPUT);
  pinMode(M1_DIR1, OUTPUT);
  pinMode(M1_DIR2, OUTPUT);
  pinMode(M2_PWR,  OUTPUT);
  pinMode(M2_DIR1, OUTPUT);
  pinMode(M2_DIR2, OUTPUT);
  pinMode(9, OUTPUT);

  //Serial.println("Ready");
  //delay(1000);
  //tcaselect(1);

}

void loop() {

  //  Serial.println("ready");

  for (int z = 1; z < 3; z++) {

    tcaselect(z);
    if (!lox.begin()) {

    }

    VL53L0X_RangingMeasurementData_t measure;
     lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if ( measure.RangeMilliMeter < 50 and (z == 1 or z == 2)) {
      //Serial.println("GO BACK!");
       //STOP = 1;
      GO = 0;
    }
    else {
      //STOP = 0;
       GO = 1;
    }

  }

    if (Serial.available()) {
      Serial.readBytes(buf, 3);
      command = buf[0];
      x = buf[1];
      y = buf[2];

      if (command == 102) {
        myservo.attach(9);
        myservo.write(x);
      }

      if (command == 101) {
        V = (100 - abs(x)) * (y / 100) + y;
        W = (100 - abs(y)) * (x / 100) + x;
        right_speed = (V + W) / 2; //result for the speed of the right motor
        left_speed = (V - W) / 2; //result for the speed of the left motor

        if (right_speed > 0 and GO == 1) {
          right_speed = map(right_speed, 0, 100, 150, TOP_SPEED);
          mot_drive_r(right_speed, FORWARD);
        }
        // motor goes reverse at a speed depending on the math above
        else {
          right_speed = map(right_speed, 0, -100, 150, TOP_SPEED);
          mot_drive_r(right_speed, REVERSE);
        }
        // motor goes forward at a speed depending on the math above
        if (left_speed > 0 and GO == 1) {
          left_speed = map(left_speed, 0, 100, 150, TOP_SPEED);
          mot_drive_l(left_speed, FORWARD);
        }
        // motor goes reverse at a speed depending on the math above
        else {
          left_speed = map(left_speed, 0, -100, 150, TOP_SPEED);
          mot_drive_l(left_speed, REVERSE);
        }
      }
    }
  }
