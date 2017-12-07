#include <Wire.h>
#include <VL53L0X.h>
#include <SoftwareSerial.h>
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
VL53L0X sensor;
VL53L0X sensor2;
VL53L0X sensor3;
VL53L0X sensor4;
VL53L0X sensor5;

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

void setup()
{
  //SPEED CONTROL
  //WGM20 = PWM, phase correct  COM0A1 = PIN 5 COM0B1 = PIN 6
  TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);
  //turns on timer0 and sets pre scale to 0
  TCCR0B = (1 << CS00);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(M1_PWR,  OUTPUT);
  pinMode(M1_DIR1, OUTPUT);
  pinMode(M1_DIR2, OUTPUT);
  pinMode(M2_PWR,  OUTPUT);
  pinMode(M2_DIR1, OUTPUT);
  pinMode(M2_DIR2, OUTPUT);

  // Open serial communications and wait for port to open:
  pinMode(9, OUTPUT);

  digitalWrite(10, LOW);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(11, LOW);

  Wire.begin();
  Serial.begin (9600);
  pinMode(11, INPUT);
  delay(150);
  sensor.init(true);
  delay(100);
  sensor.setAddress((uint8_t)22);
  pinMode(10, INPUT);
  delay(150);
  sensor2.init(true);
  delay(100);
  sensor2.setAddress((uint8_t)26);
  pinMode(4, INPUT);
  delay(150);
  sensor3.init(true);
  delay(100);
  sensor3.setAddress((uint8_t)27);
  pinMode(2, INPUT);
  delay(150);
  sensor4.init(true);
  delay(100);
  sensor4.setAddress((uint8_t)28);
  pinMode(3, INPUT);
  delay(150);
  sensor5.init(true);
  delay(100);
  sensor5.setAddress((uint8_t)29);
  / Serial.println("addresses set");
  sensor.setTimeout(100);
  sensor2.setTimeout(100);
  sensor3.setTimeout(100);
  sensor4.setTimeout(100);
  sensor5.setTimeout(100);
}

void loop()
{

  //Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;

  for (byte i = 1; i < 30; i++)
  {

    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      //Serial.print ("Found address: ");
      // Serial.print (i, DEC);
      // Serial.print (" (0x");
      // Serial.print (i, HEX);
      // Serial.println (")");
      count++;
      delay (1);  // maybe unneeded?
    } // end of good response
  } // end of for loop
  //Serial.println ("Done.");
  // Serial.print ("Found ");
  // Serial.print (count, DEC);
  // Serial.println (" device(s).");

  //CHECK DISTANCES
  long DISTANCE_FWD = (sensor.readRangeSingleMillimeters() / 25.4001);
  long DISTANCE_FWD2 = (sensor2.readRangeSingleMillimeters() / 25.4001);
  long DISTANCE_FWD3 = (sensor3.readRangeSingleMillimeters() / 25.4001);
  long DISTANCE_FWD4 = (sensor4.readRangeSingleMillimeters() / 25.4001);
  long DISTANCE_FWD5 = (sensor5.readRangeSingleMillimeters() / 25.4001);
  //FWD OR SENSOR
 /* if (sensor.timeoutOccurred())
  {
    Serial.print("Distance FWD (READING): ");
    Serial.println(" TIMEOUT");
  }
  else
  {
    Serial.print("Distance FWD   (feet): ");
    Serial.println(DISTANCE_FWD / 12);
    Serial.print("Distance FWD (inches): ");
    Serial.println(DISTANCE_FWD);
  }

  //FLT OR SENSOR2
  if (sensor2.timeoutOccurred())
  {
    Serial.print("Distance FWD2 (READING): ");
    Serial.println(" TIMEOUT");
  }
  else
  {
    Serial.print("Distance FWD2   (feet): ");
    Serial.println(DISTANCE_FWD2 / 12);
    Serial.print("Distance FWD2 (inches): ");
    Serial.println(DISTANCE_FWD2);
  }
  if (sensor3.timeoutOccurred())
  {
    Serial.print("Distance FWD3 (READING): ");
    Serial.println(" TIMEOUT");
  }
  else
  {
    Serial.print("Distance FWD3   (feet): ");
    Serial.println(DISTANCE_FWD3 / 12);
    Serial.print("Distance FWD3 (inches): ");
    Serial.println(DISTANCE_FWD3);
  }
  if (sensor4.timeoutOccurred())
  {
    Serial.print("Distance FWD4 (READING): ");
    Serial.println(" TIMEOUT");
  }
  else
  {
    Serial.print("Distance FWD4   (feet): ");
    Serial.println(DISTANCE_FWD4 / 12);
    Serial.print("Distance FWD4 (inches): ");
    Serial.println(DISTANCE_FWD4);
  }
  if (sensor5.timeoutOccurred())
  {
    Serial.print("Distance FWD5 (READING): ");
    Serial.println(" TIMEOUT");
  }
  else
  {
    Serial.print("Distance FWD5   (feet): ");
    Serial.println(DISTANCE_FWD5 / 12);
    Serial.print("Distance FWD5 (inches): ");
    Serial.println(DISTANCE_FWD5);
  }*/
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

      if (right_speed > 0) {
        right_speed = map(right_speed, 0, 100, 150, TOP_SPEED);
        //Serial.println(right_speed);
        mot_drive_r(right_speed, FORWARD);
      }
      // motor goes reverse at a speed depending on the math above
      else {
        right_speed = map(right_speed, 0, -100, 150, TOP_SPEED);
        //  Serial.println(right_speed);
        mot_drive_r(right_speed, REVERSE);
      }
      // motor goes forward at a speed depending on the math above
      if (left_speed > 0) {
        left_speed = map(left_speed, 0, 100, 150, TOP_SPEED);
        //Serial.println(left_speed);
        mot_drive_l(left_speed, FORWARD);
      }
      // motor goes reverse at a speed depending on the math above
      else {
        left_speed = map(left_speed, 0, -100, 150, TOP_SPEED);
        //Serial.println(left_speed);
        mot_drive_l(left_speed, REVERSE);
      }
    }
  }
}
