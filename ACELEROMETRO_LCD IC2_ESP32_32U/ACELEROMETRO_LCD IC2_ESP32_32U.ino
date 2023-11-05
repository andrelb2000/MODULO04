#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#define I2C_SDA 21
#define I2C_SCL 22
#define MMA8451_DEFAULT_ADDRESS (0x1C)

Adafruit_MMA8451 mma = Adafruit_MMA8451();
LiquidCrystal_I2C  lcd_i2c(0x27, 16, 2);
sensors_event_t event; 

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  if (! mma.begin(  (uint8_t) MMA8451_DEFAULT_ADDRESS) ) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");

  
 


  //------------------------//
  lcd_i2c.init();
  lcd_i2c.backlight();
  lcd_i2c.setCursor(0,0);
  lcd_i2c.print("Hello");
  //------------------------//
}
void loop() {
  mma.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  Serial.println("m/s^2 ");
  lcd_i2c.setCursor(0,0);

  if(fabs(event.acceleration.x) >= fabs(event.acceleration.y) ){
    if(fabs(event.acceleration.x) > fabs(event.acceleration.z) ){
      lcd_i2c.printf("X: %f     ",event.acceleration.x);
    }else{
      lcd_i2c.printf("Z: %f     ",event.acceleration.z);
    }
  }else{
    if(fabs(event.acceleration.y) > fabs(event.acceleration.z) ){
      lcd_i2c.printf("Y: %f     ",event.acceleration.y);
    }else{
      lcd_i2c.printf("Z: %f     ",event.acceleration.z);
    }
  }
  //lcd_i2c.printf("X:%2.2f Y:%2.2f Z:%2.2f",event.acceleration.x,event.acceleration.y,event.acceleration.z);
  
  /* Get the orientation of the sensor */
  uint8_t o = mma.getOrientation();
  
  lcd_i2c.setCursor(0,1);      
  switch (o) {
    case MMA8451_PL_PUF: 
      Serial.println("Portrait Up Front");
      lcd_i2c.print("| Up Front");      
      break;
    case MMA8451_PL_PUB: 
      Serial.println("Portrait Up Back");
      lcd_i2c.print("| Up Back");
      break;    
    case MMA8451_PL_PDF: 
      Serial.println("Portrait Down Front");
      lcd_i2c.print("| Down Front");
      break;
    case MMA8451_PL_PDB: 
      Serial.println("Portrait Down Back");
      lcd_i2c.print("| Up Back");
      break;
    case MMA8451_PL_LRF: 
      Serial.println("Landscape Right Front");
      lcd_i2c.print("= --> Front");
      break;
    case MMA8451_PL_LRB: 
      Serial.println("= --> Back");
      lcd_i2c.print("= --> Back");
      break;
    case MMA8451_PL_LLF: 
      Serial.println("= <-- Front");
      lcd_i2c.print("= <-- Front");
      break;
    case MMA8451_PL_LLB: 
      Serial.println("= <-- Back");
      lcd_i2c.print("= <-- Back");
      break;
    }
  Serial.println();
  delay(300);

}
