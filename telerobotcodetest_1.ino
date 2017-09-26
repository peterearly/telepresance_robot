
//#include <Servo.h>
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
#define FORWARD    1
#define REVERSE    2
#define STOP       3
#define F          1
#define R          2
#define S          3
int x;
#define SLAVE_ADDRESS 0x04

// set mspeed and dir to be a char
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
void setup()
{
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

  Serial.begin(9600); // start serial for output
  Serial.println("Ready!");
  Serial.println("enter a number: ");


}
void loop()
{
  //while(Serial.available()>0)
  //{
    x = Serial.read();
    if (x == 'f')
    {
    Serial.println("enter a number: ");
    mot_drive_r(200, FORWARD);
    mot_drive_l(200, FORWARD);
    }
    if (x == 'r')
    {
    Serial.println("enter a number: ");
    mot_drive_r(200, REVERSE);
    mot_drive_l(200, REVERSE);
    }
    if (x == 's')
    {
    Serial.println("enter a number: ");
    mot_drive_r(200, STOP);
    mot_drive_l(200, STOP);
    }
}
