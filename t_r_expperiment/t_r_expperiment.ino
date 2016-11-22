#include <Wire.h>


#include <Servo.h>


#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;
//int time = 50;
//int time2 = 55;
int analogPin = 3;   // potentiometer connected to analog pin 3


int val = 0;         // variable to store the read value


void setup()
{
	pinMode(13, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(11, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(5, OUTPUT);
	
	
	Serial.begin(9600); // start serial for output
// initialize i2c as slave
	Wire.begin(SLAVE_ADDRESS);


// define callbacks for i2c communication
	Wire.onReceive(receiveData);
	Wire.onRequest(sendData);


	Serial.println("Ready!");
}


void loop()
{
	delay(100);
}


// callback for received data
void receiveData(int byteCount)
{
 
	while(Wire.available())
	{
		number = Wire.read();
		Serial.print("data received: ");
		Serial.println(number);


		if (number == 1){
		val = analogRead(analogPin);   
		analogWrite(9, 100);  
		analogWrite(5, 200);  




		
		digitalWrite(12, LOW);
		digitalWrite(13, HIGH);
		digitalWrite(11, HIGH);
		digitalWrite(10, LOW);
		}
		
	if (number == 0){
		digitalWrite(5, LOW);
		digitalWrite(9, LOW);
}
		if (number == 2){
		val = analogRead(analogPin);   
		analogWrite(9, 100);  
		analogWrite(5, 125); 
		
		digitalWrite(12, HIGH);
		digitalWrite(13, LOW);
		digitalWrite(11, LOW);
		digitalWrite(10, HIGH);
	
		}
		if (number == 3){
	//	val = analogRead(analogPin);   
		analogWrite(5, HIGH); 
		digitalWrite(9, LOW);
		digitalWrite(12, HIGH);
		digitalWrite(13, LOW);
		
		}
		if (number == 4){
	//	val = analogRead(analogPin);   
		analogWrite(9, HIGH); 
		digitalWrite(5, LOW);
		digitalWrite(11, HIGH);
		digitalWrite(10, LOW);
		
		}
	}	
}




// callback for sending data
void sendData()
{
	Wire.write(number);
}



