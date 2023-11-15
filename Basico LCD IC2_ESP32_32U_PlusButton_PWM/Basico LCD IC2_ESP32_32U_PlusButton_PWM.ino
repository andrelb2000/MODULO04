#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define I2C_SDA 21
#define I2C_SCL 22
#define PUSH_BUTTON1 12

#define PERIODO_50HZ 20
#define CANALDEFAULT  0
#define FREQDEFAULT  50
#define MAXBITSRESOLUCAO 1024 
#define RESOLUCAO 10
#define PIN_SERVO 27

#define PIN_POT 34



LiquidCrystal_I2C  lcd_i2c(0x27, 16, 2);
int buttonState = true;
int contador = 0;
int duty = 50;
int potValue = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  //------------------------//
  lcd_i2c.init();
  lcd_i2c.backlight();
  lcd_i2c.setCursor(0,0);
  lcd_i2c.print("Hello");
  //------------------------//
  pinMode(PUSH_BUTTON1,INPUT);
  // -----------------------//
  ledcAttachPin(PIN_SERVO, CANALDEFAULT);
  ledcSetup(CANALDEFAULT, FREQDEFAULT, RESOLUCAO);

  pinMode(PIN_POT,INPUT);
  analogReadResolution(16);
}
void loop() {
  int button1Pressed = digitalRead(PUSH_BUTTON1);
  contador++;

  potValue = analogRead(PIN_POT);
  duty = potValue / 500 + 30;
  Serial.printf("Potenciometro: %i\n",potValue);
  if(button1Pressed){
    if(buttonState){
      buttonState = false;
      // Faz algo
      duty = 30;
      lcd_i2c.setCursor(0,1);
      lcd_i2c.printf("Cnt %3i-Duty %3i",contador,duty);
      
      ledcWrite(CANALDEFAULT,duty);
    }
  }else{
    buttonState = true;
    ledcWrite(CANALDEFAULT,duty);
  }
  lcd_i2c.setCursor(0,0);
  lcd_i2c.printf("Cnt %3i-Duty %3i",contador,duty);
  delay(500);
 
}
