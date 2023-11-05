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
int pinSOM[] = {NOTE_G1,NOTE_D2,NOTE_G2,NOTE_G3};
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

  if (pinoPressionado){
    lcd_i2c.printf("PRESS pin:%i-%i   ", pinButton[indice], pinoLed[indice]);
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
}
