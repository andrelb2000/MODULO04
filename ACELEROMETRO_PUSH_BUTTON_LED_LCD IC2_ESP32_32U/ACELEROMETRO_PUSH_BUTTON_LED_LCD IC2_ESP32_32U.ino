#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#define NOTE_G3  196
#define I2C_SDA 21
#define I2C_SCL 22
#define MMA8451_DEFAULT_ADDRESS (0x1C)

#define NAO_PRESSIONADO_NAO_TOCANDO 0 
#define PRESSIONADO_TOCANDO 1
#define PRESSIONADO_NAO_TOCANDO 2

Adafruit_MMA8451 mma = Adafruit_MMA8451();
LiquidCrystal_I2C  lcd_i2c(0x27, 16, 2);
sensors_event_t event; 

//(E1*) --> Pino nao pressionado nao tocando
//(E2*) --> Pino pressionado tocando
//(E3*) --> Pino pressionado nao tocando

int LeituraNr = 0;
//( E1* ) --> ( E2 ) --> ( E3 )
int estadoPino1 = NAO_PRESSIONADO_NAO_TOCANDO;
int estadoPino[] = {NAO_PRESSIONADO_NAO_TOCANDO,
                    NAO_PRESSIONADO_NAO_TOCANDO,
                    NAO_PRESSIONADO_NAO_TOCANDO,
                    NAO_PRESSIONADO_NAO_TOCANDO };

//int pinoPressionado[] = {0,0,0,0};
int pinoNrToques[] = {0,0,0,0};
int pinoLed[] = {19,18,5,17};
int pinButton[] = {25,26,27,14};
int pino1Buzz = 16;
int capturar = 0;
float xcap,ycap,zcap;
int orientationcap;

int lePino(int indice){
  int pinoPressionado = digitalRead(pinButton[indice]); 
  int foiPressionado = 0;
  if (pinoPressionado){
    //( E1* ) --> ( E2 ) --> ( E3 ) --> ( E4 )
    if (estadoPino[indice] == NAO_PRESSIONADO_NAO_TOCANDO){
      //( E1 ) --> ( E2* ) --> ( E3 )--> ( E3 )  
      estadoPino[indice] = PRESSIONADO_TOCANDO;    
    }      
  }

  //( E1 ) --> ( E2* ) --> ( E3 )     
  if (estadoPino[indice] == PRESSIONADO_TOCANDO){
    digitalWrite(pinoLed[indice], HIGH);
    tone(pino1Buzz,NOTE_G3,100);
    noTone(pino1Buzz); 
    pinoNrToques[indice]++;
    if(capturar==2){
      capturar = 0;  
    }else{
      capturar = 1;
    }
    
  }

  //( E1 ) --> ( E2 ) --> ( E3* )     
  if (estadoPino[indice] == PRESSIONADO_NAO_TOCANDO){
    digitalWrite(pinoLed[indice], LOW);  
    foiPressionado = 1;
  }

  if (pinoPressionado){
    Serial.printf("PRESS pin:%i-%i   ", pinButton[indice], pinoLed[indice]);
    Serial.printf("N/PRESS           ");
  }else{
    Serial.printf("N/PRESS        ");
  }

  //( E1 ) --> ( E2* ) --> ( E3 ) 
  if(estadoPino[indice]==PRESSIONADO_TOCANDO){
    //( E1 ) --> ( E2 ) --> ( E3* )     
    estadoPino[indice] = PRESSIONADO_NAO_TOCANDO;
  }

  //( E1 ) --> ( E2 ) --> ( E3* )
  if(estadoPino[indice]==PRESSIONADO_NAO_TOCANDO && pinoPressionado==0){
    //( E1* ) --> ( E2 ) --> ( E3 )
    estadoPino[indice] = NAO_PRESSIONADO_NAO_TOCANDO;
  }
  return (foiPressionado);
};


void setup() {
  for(int i=0 ; i < 4; i++){
    pinMode(pinButton[i], INPUT);    
    pinMode(pinoLed[i],  OUTPUT);    
    pinoNrToques[i] = 0;
  }  
  pinMode(pino1Buzz, OUTPUT);  
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

  for(int i=0 ; i < 4; i++){
    lePino(i);      
  } 

  if(capturar){
      if(capturar!=2){
        xcap = event.acceleration.x;
        ycap = event.acceleration.y;
        zcap = event.acceleration.z;
      }
      capturar = 2;
      if(capturar){
      lcd_i2c.setCursor(0,0);
      lcd_i2c.printf("%2.2f %2.2f %2.2f",xcap,ycap,zcap);
      }  
  }else{
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
  } 
  
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
