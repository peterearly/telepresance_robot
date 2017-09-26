//#include <Wire.h>
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
#define FORWARD    1
#define REVERSE    2
#define STOP       3
#define F          1
#define R          2
#define S          3
int x = 0;
const int anPin = 1;
const int anPin2 = 2;
const int anPin3 = 3;
const int anPin4 = 4;

long anVolt, inches, cm;
long anVolt2, inches2, cm2;
long anVolt3, inches3, cm3;
long anVolt4, inches4, cm4;
int sum = 0;//Create sum variable so it can be averaged
int sum2 = 0;
int sum3 = 0;//Create sum variable so it can be averaged
int sum4 = 0;
int avgrange = 10; //Quantity of values to average (sample size)




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
  pinMode(10, OUTPUT); 
  
  Serial.begin(9600); // start serial for output
  Serial.println("Ready!");


}
void loop()
{
  char recData[1];
  char inByte = ' ';
  if(Serial.available())
  {
   recData[0] = Serial.read();
    // Serial.readBytesUntil(255,recData,3);
      //unsigned char recData[2];
      //Serial.print(x);
      //int inByte = Serial.read();
      //  Serial.println(inByte);
     // for(x=0; x<=2; x++)
     //   recData[x]= Serial.read();

      //if (recData[0] < 255 & recData[1] < 255){
      Serial.print("rsv: ");
      Serial.print (recData[0] ,DEC);
     // Serial.print(" lsv: ");
     // Serial.print (recData[1],DEC);
        Serial.print(" | ");

      //unsigned char RM = recData[1];
      unsigned char LM = recData[0];

      if (LM < 0){
        LM = map(LM, 0, -100, 50, 255);
        mot_drive_r(LM, REVERSE);
        mot_drive_l(LM, REVERSE);
      } 
      else{
        LM = map(LM, 0, 100, 50, 255);
        mot_drive_r(LM, FORWARD);
        mot_drive_l(LM, FORWARD);

      } 
     
  }
  for (int i = 0; i < avgrange ; i++)

  {
    digitalWrite(10, HIGH);
    delayMicroseconds(20);
    digitalWrite(10,LOW);
    anVolt = analogRead(anPin) / 2;
    sum += anVolt;
    anVolt2 = analogRead(anPin2) / 2;
    sum2 += anVolt2;
    anVolt3 = analogRead(anPin3) / 2;
    sum3 += anVolt3;
    anVolt4 = analogRead(anPin4) / 2;
    sum4 += anVolt4;
    delay(10);
  }
  inches = sum / avgrange;
  cm = inches * 2.54;
  inches2 = sum2 / avgrange;
  cm2 = inches2 * 2.54;
  inches3 = sum3 / avgrange;
  cm3 = inches3 * 2.54;
  inches4 = sum4 / avgrange;
  cm4 = inches4 * 2.54;

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");

  Serial.print(" | ");

  Serial.print(inches2);
  Serial.print("in, ");
  Serial.print(cm2);
  Serial.print("cm");

  Serial.print(" | ");

  Serial.print(inches3);
  Serial.print("in, ");
  Serial.print(cm3);
  Serial.print("cm");

  Serial.print(" | ");

  Serial.print(inches4);
  Serial.print("in, ");
  Serial.print(cm4);
  Serial.print("cm");
  Serial.println();

//reset sample total

  sum = 0;
  sum2 = 0;
  sum3 = 0;
  sum4 = 0;
  if (inches <= 6 || inches2 <=6){
      mot_drive_r(250, FORWARD);
      mot_drive_l(250, FORWARD);

      delay(500);
      mot_drive_r(0, FORWARD);
      mot_drive_l(0, FORWARD);



  }
 if (inches3 <= 6 || inches4 <= 6){
      mot_drive_r(250, REVERSE);
      mot_drive_l(250, REVERSE);

      delay(500);
      mot_drive_r(0, REVERSE);
      mot_drive_l(0, REVERSE);

  }
  delay(500);
}






