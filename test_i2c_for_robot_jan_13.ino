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

  Serial.begin(115200); // start serial for output

  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");
 
}
void loop()
{


}
// callback for received data
void receiveData(int byteCount)
{
  unsigned char recData[2];
  int x;
  while(Wire.available()) {

    for(x=0; x<2; x++)
      recData[x]= Wire.read();

    Serial.print("rsv: ");
    Serial.print (recData[0]);
    Serial.print(" lsv: ");
    Serial.print (recData[1]);
    unsigned char RM = recData[0];
    unsigned char LM = recData[1];

    if (RM > 100){
      RM = map(RM, 100, 200, 50, 255);
      mot_drive_r(RM, REVERSE);
    } 
    else{
      RM = map(RM, 100, 0, 50, 255);
      mot_drive_r(RM, FORWARD);
    } 
    if (LM > 100){
      LM = map(LM, 100,200,50,255);
      mot_drive_l(LM, REVERSE);
    } 
    else{
      LM = map(LM, 100,0,50,255);
      mot_drive_l(LM, FORWARD);
    } 
  }
}
// callback for sending data
void sendData()
{
  // Wire.write(number);
}





