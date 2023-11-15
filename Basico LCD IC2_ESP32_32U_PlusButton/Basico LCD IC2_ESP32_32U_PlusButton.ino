#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define I2C_SDA 21
#define I2C_SCL 22
#define PUSH_BUTTON1 12
LiquidCrystal_I2C  lcd_i2c(0x27, 16, 2);
int buttonState = true;
int contador = 0;

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
}
void loop() {
  int button1Pressed = digitalRead(PUSH_BUTTON1);
  contador++;

  if(button1Pressed){
    if(buttonState){
      buttonState = false;
      // Faz algo
       lcd_i2c.setCursor(0,1);
      lcd_i2c.printf("PRESSIONADO %i",contador);
    }
  }else{
    buttonState = true;
  }
  lcd_i2c.setCursor(0,0);
  lcd_i2c.printf("CONTANDO %i",contador);
  delay(500);
 
}
