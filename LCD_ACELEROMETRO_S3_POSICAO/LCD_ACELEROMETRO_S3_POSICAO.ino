#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Adafruit_MMA8451.h"
#include <Adafruit_Sensor.h>

#define MMA8451_DEFAULT_ADDRESS (0x1C)



#define I2C_SDA  47
#define I2C_SCL  48
#define ACELL_TRESH 0.3



LiquidCrystal_I2C  lcd_i2cBase(0x27, 16, 2);
//////////////////////////////
class MostradorLCD {
  private:
    LiquidCrystal_I2C *lcd_i2c;
  public:
    MostradorLCD (LiquidCrystal_I2C *lcd){
      lcd_i2c = lcd;     
      lcd_i2c->init(); // initialize the lcd
      lcd_i2c->backlight();

    };
    void mostraL1(char *texto){
      /*
      Serial.printf("L1: %s\n",texto);
      lcd_i2c->setCursor(0, 0); 
      lcd_i2c->printf("%16s",texto);
      */
    };
    void mostraL1b(float x,float v){
      lcd_i2c->setCursor(0, 0); 
      lcd_i2c->printf("X:%2.1fm Vx:%2.1f",x,v);
    };
    
    void mostraL2(char *texto){
      lcd_i2c->setCursor(0, 1); 
       lcd_i2c->printf("%16s",texto);
    };
    void mostraL2XYZ(float x, float y, float z){
      lcd_i2c->setCursor(0, 1); 
      lcd_i2c->printf("X:%1.1f Y:%1.1f Z:%1.1f",x,y,z);
    };
};
MostradorLCD *lcd = NULL;

Adafruit_MMA8451 mma = Adafruit_MMA8451();

float inicioX,fimX,deltaTX;
float acelX;
float vX = 0;
float sX = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  //------------------------//
  lcd = new MostradorLCD(&lcd_i2cBase);
  //------------------------//

  Serial.println("Adafruit MMA8451 test!");
  byte error, address;
  Serial.println("Scanning...");
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device at address:");
      Serial.println(address,HEX); 
    }
  }


  if (!mma.begin((uint8_t) MMA8451_DEFAULT_ADDRESS)) {
    Serial.println("Couldnt start");
    while(1);
  }
  Serial.println("MMA8451 found!");
  mma.setRange(MMA8451_RANGE_2_G);
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");
}
void loop() {
  // Read the 'raw' data in 14-bit 
  inicioX = micros();
  mma.read();
  mma.setRange(MMA8451_RANGE_2_G);
  /*
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");
  Serial.print("X:\t");   Serial.print(mma.x); 
  Serial.print("\tY:\t"); Serial.print(mma.y); 
  Serial.print("\tZ:\t"); Serial.print(mma.z); 
  Serial.println();
*/
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) 
  Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  Serial.println("m/s^2 ");*/
  lcd->mostraL2XYZ(event.acceleration.x,event.acceleration.y,event.acceleration.z);
  if(fabs(event.acceleration.x) > ACELL_TRESH){
    acelX = event.acceleration.x;
  }else{
    acelX = 0;
  }
  
  
  fimX = micros();
  Serial.printf("InicioX:%f FimX:%f detaTX:%f vX:%f acelX:%f",inicioX,fimX,deltaTX,vX,acelX);
  deltaTX = (fimX - inicioX)/1000000;
  sX += vX*deltaTX + acelX * deltaTX * deltaTX /2.0;
  if(fabs(acelX) < ACELL_TRESH){
    vX = 0;
  }else{
    vX += acelX*deltaTX;
  }
  
  lcd->mostraL1b(sX,vX);
  Serial.printf("Andou %lf m ate agora \n",sX); 


  /* Get the orientation of the sensor 
  uint8_t o = mma.getOrientation();
  
  switch (o) {
    case MMA8451_PL_PUF: 
      lcd->mostraL1("Frente,Cima,Vert");
      Serial.println("Portrait Up Front");
      break;
    case MMA8451_PL_PUB: 
      lcd->mostraL1("Tras,Cima,Vert");
      Serial.println("Portrait Up Back");
      break;    
    case MMA8451_PL_PDF: 
      lcd->mostraL1("Frent,Baixo,Vert");
      Serial.println("Portrait Down Front");
      break;
    case MMA8451_PL_PDB: 
      lcd->mostraL1("Tras,Cima,Vert");   
      Serial.println("Portrait Down Back");
      break;
    case MMA8451_PL_LRF: 
      lcd->mostraL1("Frent,Dir,Horiz");   
      Serial.println("Landscape Right Front");
      break;
    case MMA8451_PL_LRB: 
      lcd->mostraL1("Tras,Dir,Horiz");
      Serial.println("Landscape Right Back");
      break;
    case MMA8451_PL_LLF:
      lcd->mostraL1("Frent,Esq,Horiz"); 
      Serial.println("Landscape Left Front");
      break;
    case MMA8451_PL_LLB: 
      lcd->mostraL1("Tras,Esq,Horiz"); 
      Serial.println("Landscape Left Back");
      break;
    }
  Serial.println();*/
  

}
