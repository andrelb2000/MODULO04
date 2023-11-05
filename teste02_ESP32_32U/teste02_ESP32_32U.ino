#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd_i2c(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
const int I2C_SDA = 21;
const int I2C_SCL = 22;
const int pino35out = 33;

void setup() {
  // put your setup code here, to run once:
  pinMode(pino35out,OUTPUT);
  digitalWrite(pino35out,HIGH);

  Serial.begin(115200);
  Serial.println("Iniciando a serial");
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd_i2c.init(); // initialize the lcd
  lcd_i2c.backlight();

}

void loop() {
  // put your main code here, to run repeatedly:
  lcd_i2c.clear();

  digitalWrite(pino35out, LOW);
  Serial.println("Apagando");
  lcd_i2c.setCursor(0, 1);
  lcd_i2c.printf("Apagando");
  delay(1000);
  digitalWrite(pino35out, HIGH);
  Serial.println("Acendendo");
  lcd_i2c.setCursor(0, 1);
  lcd_i2c.printf("Acendendo");
  delay(1000);

}
