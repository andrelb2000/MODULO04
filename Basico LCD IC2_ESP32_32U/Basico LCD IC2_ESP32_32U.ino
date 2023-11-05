#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define I2C_SDA  40
#define I2C_SCL  39
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
      Serial.printf("L1: %s\n",texto);
      lcd_i2c->setCursor(0, 0); 
      lcd_i2c->printf(texto);
    };
    void mostraL2(char *texto){
      lcd_i2c->setCursor(0, 1); 
      lcd_i2c->printf(texto);
    };
    void mostraL2Distancia(int dist){
      lcd_i2c->setCursor(0, 1); 
      lcd_i2c->printf("dist:%4i cm",dist);
    };

};
MostradorLCD *lcd = NULL;


void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  //------------------------//
  lcd = new MostradorLCD(&lcd_i2cBase);
  //------------------------//
}
void loop() {
  lcd->mostraL1("Teste Linha 1");
  lcd->mostraL2("Teste Linha 2");
  delay(1000);
  lcd->mostraL2Distancia(35);
  delay(1000);  
}
