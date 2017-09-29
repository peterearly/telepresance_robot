#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;
VL53L0X sensor2;
VL53L0X sensor3;
VL53L0X sensor4;
VL53L0X sensor5;
VL53L0X sensor6;

void setup()
{

  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(11, LOW);


  delay(500);
  Wire.begin();


  Serial.begin (9600);

  //SENSOR
  pinMode(11, INPUT);
  delay(150);
  Serial.println("00");
  sensor.init(true);
  Serial.println("01");
  delay(100);
  sensor.setAddress((uint8_t)22);
  Serial.println("02");

  //SENSOR 2
  pinMode(9, INPUT);
  delay(150);
  sensor2.init(true);
  Serial.println("03");
  delay(100);
  sensor2.setAddress((uint8_t)25);
  Serial.println("04");
  
  pinMode(10, INPUT);
  delay(150);
  sensor3.init(true);
  Serial.println("03");
  delay(100);
  sensor3.setAddress((uint8_t)26);
  Serial.println("04");

pinMode(4, INPUT);
  delay(150);
  sensor4.init(true);
  Serial.println("03");
  delay(100);
  sensor4.setAddress((uint8_t)27);
  Serial.println("04");

pinMode(2, INPUT);
  delay(150);
  sensor5.init(true);
  Serial.println("03");
  delay(100);
  sensor5.setAddress((uint8_t)28);
  Serial.println("04");

pinMode(3, INPUT);
  delay(150);
  sensor6.init(true);
  Serial.println("03");
  delay(100);
  sensor6.setAddress((uint8_t)29);
  Serial.println("04");


  Serial.println("");
  Serial.println("addresses set");
  Serial.println("");
  Serial.println("");
  



  sensor.setTimeout(500);
  sensor2.setTimeout(500);
  sensor3.setTimeout(500);
  sensor4.setTimeout(500);
  sensor5.setTimeout(500);
  sensor6.setTimeout(500);

}

void loop()
{
  Serial.println("__________________________________________________________________");
  Serial.println("");
  Serial.println("=================================");
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;


  //for (byte i = 1; i < 120; i++)
  for (byte i = 1; i < 30; i++)
  {

    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);  // maybe unneeded?
    } // end of good response
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
  Serial.println("=================================");


  //CHECK DISTANCES
  long DISTANCE_FWD = (sensor.readRangeSingleMillimeters()/25.4001);
  long DISTANCE_FWD2 = (sensor2.readRangeSingleMillimeters()/25.4001);
  long DISTANCE_FWD3 = (sensor3.readRangeSingleMillimeters()/25.4001);
  long DISTANCE_FWD4 = (sensor4.readRangeSingleMillimeters()/25.4001);
  long DISTANCE_FWD5 = (sensor5.readRangeSingleMillimeters()/25.4001);
  long DISTANCE_FWD6 = (sensor6.readRangeSingleMillimeters()/25.4001);

  //FWD OR SENSOR
  if (sensor.timeoutOccurred())
  {
  //  Serial.println("_________________________________");
    Serial.print("Distance FWD (READING): ");
    Serial.println(" TIMEOUT");
    Serial.println("_________________________________");
    Serial.println("");
  }
  else
  {
  //  Serial.println("_________________________________");
    Serial.print("Distance FWD   (feet): ");
    Serial.println(DISTANCE_FWD/12);
    Serial.print("Distance FWD (inches): ");
    Serial.println(DISTANCE_FWD);
    Serial.println("_________________________________");
    Serial.println("");
  }

  //FLT OR SENSOR2
  if (sensor2.timeoutOccurred())
  {
 //   Serial.println("_________________________________");
    Serial.print("Distance FWD2 (READING): ");
    Serial.println(" TIMEOUT");
    Serial.println("_________________________________");
    Serial.println("");
  }
  else
  {
  //  Serial.println("_________________________________");
    Serial.print("Distance FWD2   (feet): ");
    Serial.println(DISTANCE_FWD2/12);
    Serial.print("Distance FWD2 (inches): ");
    Serial.println(DISTANCE_FWD2);
    Serial.println("_________________________________");
    Serial.println("");
  }
  if (sensor3.timeoutOccurred())
  {
   // Serial.println("_________________________________");
    Serial.print("Distance FWD3 (READING): ");
    Serial.println(" TIMEOUT");
    Serial.println("_________________________________");
    Serial.println("");
  }
  else
  {
   // Serial.println("_________________________________");
    Serial.print("Distance FWD3   (feet): ");
    Serial.println(DISTANCE_FWD3/12);
    Serial.print("Distance FWD3 (inches): ");
    Serial.println(DISTANCE_FWD3);
    Serial.println("_________________________________");
    Serial.println("");
  }
  if (sensor4.timeoutOccurred())
  {
    //Serial.println("_________________________________");
    Serial.print("Distance FWD4 (READING): ");
    Serial.println(" TIMEOUT");
    Serial.println("_________________________________");
    Serial.println("");
  }
  else
  {
  //  Serial.println("_________________________________");
    Serial.print("Distance FWD4   (feet): ");
    Serial.println(DISTANCE_FWD4/12);
    Serial.print("Distance FWD4 (inches): ");
    Serial.println(DISTANCE_FWD4);
    Serial.println("_________________________________");
    Serial.println("");
  }
  if (sensor5.timeoutOccurred())
  {
  //  Serial.println("_________________________________");
    Serial.print("Distance FWD5 (READING): ");
    Serial.println(" TIMEOUT");
    Serial.println("_________________________________");
    Serial.println("");
  }
  else
  {
 //   Serial.println("_________________________________");
    Serial.print("Distance FWD5   (feet): ");
    Serial.println(DISTANCE_FWD5/12);
    Serial.print("Distance FWD5 (inches): ");
    Serial.println(DISTANCE_FWD5);
    Serial.println("_________________________________");
    Serial.println("");
  }
if (sensor6.timeoutOccurred())
  {
  //  Serial.println("_________________________________");
    Serial.print("Distance FWD6 (READING): ");
    Serial.println(" TIMEOUT");
    Serial.println("_________________________________");
    Serial.println("");
  }
  else
  {
  // Serial.println("_________________________________");
    Serial.print("Distance FWD6   (feet): ");
    Serial.println(DISTANCE_FWD6/12);
    Serial.print("Distance FWD6 (inches): ");
    Serial.println(DISTANCE_FWD6);
    Serial.println("_________________________________");
    Serial.println("");
  }  
  Serial.println("__________________________________________________________________");
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();


  
  delay(2000);//can change to a lower time like 100
}
