#include <LiquidCrystal_I2C.h>
#include <Wire.h>



#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978




#define I2C_SDA 21
#define I2C_SCL 22

#define NAO_PRESSIONADO_NAO_TOCANDO 0 
#define PRESSIONADO_TOCANDO 1
#define PRESSIONADO_NAO_TOCANDO 2

LiquidCrystal_I2C  lcd_i2c(0x27, 16, 2);


//(E1*) --> Pino nao pressionado nao tocando
//(E2*) --> Pino pressionado tocando
//(E3*) --> Pino pressionado nao tocando

int LeituraNr = 0;
//( E1* ) --> ( E2 ) --> ( E3 )
int estadoPino[] = {NAO_PRESSIONADO_NAO_TOCANDO,
                    NAO_PRESSIONADO_NAO_TOCANDO,
                    NAO_PRESSIONADO_NAO_TOCANDO,
                    NAO_PRESSIONADO_NAO_TOCANDO };

//int pinoPressionado[] = {0,0,0,0};
int pinoNrToques[] = {0,0,0,0};
int pinoLed[] = {19,18,5,17};
int pinButton[] = {25,26,27,14};
int pinSOM[] = {NOTE_G3,NOTE_G5,NOTE_G6,NOTE_G7};
int pinoBuzz = 16;

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
    tone(pinoBuzz,pinSOM[indice],100);
    noTone(pinoBuzz); 
    pinoNrToques[indice]++;   
  }

  //( E1 ) --> ( E2 ) --> ( E3* )     
  if (estadoPino[indice] == PRESSIONADO_NAO_TOCANDO){
    digitalWrite(pinoLed[indice], LOW);  
    foiPressionado = 1;
  }
  lcd_i2c.setCursor(0, 0);
  if (pinoPressionado){
    lcd_i2c.printf("PRESS pin:%i-%i   ", pinButton[indice], pinoLed[indice]); 
    delay(200);    
  }else{
    lcd_i2c.printf("N/PRESS        ");
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
  pinMode(pinoBuzz, OUTPUT);  
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  //------------------------//
  lcd_i2c.init();
  lcd_i2c.backlight();
  lcd_i2c.setCursor(0,0);
  lcd_i2c.print("Hello");
  //------------------------//
}
void loop() {
  lcd_i2c.setCursor(0,0);
  for(int i=0 ; i < 4; i++){
    lePino(i);      
    lcd_i2c.setCursor(i*4, 1);
    lcd_i2c.printf("(%i) ",pinoNrToques[i]);        
  } 
  delay(200);
}
