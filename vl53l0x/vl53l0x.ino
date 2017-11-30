#include "Adafruit_VL53L0X.h"
#define TCAADDR 0x70
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int x = 1;

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}
void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
 // while (! Serial) {
//    delay(1);
//  }
  
  //Serial.println("Adafruit VL53L0X test");
  
  // power 
  //Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  tcaselect(x);
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
  //  while(1);
  
  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
    
  delay(100);
}
}
