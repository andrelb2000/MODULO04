#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define I2C_SDA 21
#define I2C_SCL 22
LiquidCrystal_I2C  lcd_i2c(0x27, 16, 2);

void setup() {
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
 
}
