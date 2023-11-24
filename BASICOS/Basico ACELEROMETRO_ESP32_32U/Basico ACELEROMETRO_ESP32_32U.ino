#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#define I2C_SDA 21
#define I2C_SCL 22
#define MMA8451_DEFAULT_ADDRESS (0x1C)

Adafruit_MMA8451 mma = Adafruit_MMA8451();
sensors_event_t event; 
int statusAcelerometro = true;

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  if (! mma.begin(  (uint8_t) MMA8451_DEFAULT_ADDRESS) ) {
    Serial.println("Couldnt start");
    statusAcelerometro = false;
  }else{
    Serial.println("MMA8451 found!");
    statusAcelerometro = true;
  }  
}
void loop() {
  if(statusAcelerometro){
    mma.getEvent(&event);
    /* Show the results (acceleration is measured in m/s^2) */
    Serial.printf("X:%2.3f m/s^2 Y:%2.3f m/s^2 Z:%2.3f m/s^2\n",
                  event.acceleration.x,event.acceleration.y,event.acceleration.z); Serial.print("\t");
    if(fabs(event.acceleration.x) >= fabs(event.acceleration.y) ){
      if(fabs(event.acceleration.x) > fabs(event.acceleration.z) ){
        Serial.printf("Maior aceleração X: %f     ",event.acceleration.x);
      }else{
        Serial.printf("Maior aceleração Z: %f     ",event.acceleration.z);
      }
    }else{
      if(fabs(event.acceleration.y) > fabs(event.acceleration.z) ){
        Serial.printf("Maior aceleração Y: %f     ",event.acceleration.y);
      }else{
        Serial.printf("Maior aceleração Z: %f     ",event.acceleration.z);
      }
    }
    /* Get the orientation of the sensor */
    uint8_t o = mma.getOrientation();    
    switch (o) {
      case MMA8451_PL_PUF: 
        Serial.println("Portrait Up Front");
        break;
      case MMA8451_PL_PUB: 
        Serial.println("Portrait Up Back");
        break;    
      case MMA8451_PL_PDF: 
        Serial.println("Portrait Down Front");
        break;
      case MMA8451_PL_PDB: 
        Serial.println("Portrait Down Back");
        break;
      case MMA8451_PL_LRF: 
        Serial.println("Landscape Right Front");
        break;
      case MMA8451_PL_LRB: 
        Serial.println("= --> Back");
        break;
      case MMA8451_PL_LLF: 
        Serial.println("= <-- Front");
        break;
      case MMA8451_PL_LLB: 
        Serial.println("= <-- Back");
        break;
      }
    Serial.println();
    delay(300);
  }


}
