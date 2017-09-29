#include <Wire.h>
#include <Servo.h>
#include <avr/io.h> // includes external libraries
#include <avr/interrupt.h>


#define M1_PWR     5      //physical pin 11
#define M2_PWR     6      //physical pin 12
#define M1_DIR1    7      //physical pin 13
#define M1_DIR2    8     //physical pin 14
#define M2_DIR1    12     //physical pin 18
#define M2_DIR2    13      //physical pin 19

#define TOP_SPEED 235

#define FORWARD  10
#define REVERSE  20
#define STOP     30

#define YAXIS 2
#define XAXIS 1
#define SLAVE_ADDRESS 0x04

// set mspeed and dir to be a char
void mot_drive_r(char mspeed,char dir)
{
  OCR0B = mspeed;
  if (dir == REVERSE){    
    digitalWrite(M1_DIR1, HIGH);
    digitalWrite(M1_DIR2, LOW);
  }
  if (dir == STOP){
    digitalWrite(M1_DIR1, LOW);
    digitalWrite(M1_DIR2, LOW);
  }
  if (dir == FORWARD){
    digitalWrite(M1_DIR1, LOW);
    digitalWrite(M1_DIR2, HIGH);//REVERSED
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
void receiveData(int byteCount)
{ 
  int left_speed; // int for the speed of the left motor
  int right_speed; // int for the speed of the right motor
  int V; // a variable used for the math
  int W; // a variable used for the math
  int8_t yaxis; // variable for the y-axis recieved from the app
  int command; 
  int8_t xaxis; // variable for the x-axis recieved from the app
  while(Wire.available()) { 
    command = Wire.read();
    
    if (command == XAXIS){
      xaxis = Wire.read();
      yaxis = Wire.read();
      Serial.print("x-axis: ");
      Serial.println(xaxis);
      Serial.print("y-axis: ");
      Serial.println(yaxis);
     
     // Math for converting to tank drive for -100 to 100
     // results depend on the xaxis and yaxis recieved from joystick
      V = (100 - abs(xaxis)) * (yaxis/100) + yaxis;
      W = (100 - abs(yaxis)) * (xaxis/100) + xaxis;
      right_speed = (V+W) /2; //result for the speed of the right motor
      left_speed = (V-W) /2; //result for the speed of the left motor
      
      Serial.print("V: ");
      Serial.println(right_speed);
      Serial.print("W: ");
      Serial.println(left_speed);
      
      // motor goes forward at a speed depending on the math above
      if(right_speed > 0){
        right_speed = map(right_speed, 0, 100, 150, TOP_SPEED);
       //  Serial.println(right_speed);
        mot_drive_r(right_speed, FORWARD); //DOING OPPOSITE
      }
      // motor goes reverse at a speed depending on the math above
      else{
        right_speed = map(right_speed, 0, -100, 150, TOP_SPEED);
        //Serial.println(right_speed);
        mot_drive_r(right_speed, REVERSE);
      }
      // motor goes forward at a speed depending on the math above      
      if(left_speed > 0){
        left_speed = map(left_speed, 0, 100, 150, TOP_SPEED);
       // Serial.println(left_speed);
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
}
void sendData()
{
  // Wire.write(number);
}
