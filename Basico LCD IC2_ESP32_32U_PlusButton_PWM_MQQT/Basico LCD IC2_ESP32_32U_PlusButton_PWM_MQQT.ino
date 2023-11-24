#include <PubSubClient.h>
#include <UbiConstants.h>
#include <UbiTypes.h>
#include <UbidotsEsp32Mqtt.h>

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



//LiquidCrystal_I2C  lcd_i2c(0x27, 16, 2);
int buttonState = true;
int contador = 0;
int duty = 50;
int potValue = 0;

/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBUS-Omv5uCEHXbcyX6yicsOBgzZG3AMgOh";  // Put here your Ubidots TOKEN
                      
//const char *WIFI_SSID = "Inteli-COLLEGE";      // Put here your Wi-Fi SSID
//const char *WIFI_PASS = "QazWsx@123";      // Put here your Wi-Fi password

const char *WIFI_SSID = "AMM_NOVOS_HORIZONTES_5G";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "melissabraga";      // Put here your Wi-Fi password

const char *DEVICE_LABEL = "ALB_NOTE001";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "ALB_DUTY001"; // Put here your Variable label to which data  will be published

const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds
Ubidots ubidots(UBIDOTS_TOKEN);
//unsigned long timer;

void callback(char *topic, byte *payload, unsigned int length){
  Serial.print("Message arrived [");
  /*
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
  }
  */
  Serial.println();
}


void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_SDA, I2C_SCL);
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  if (!ubidots.connected())  {
    ubidots.reconnect();
    Serial.println("Reconectando");
  }
  //------------------------//
  /*
  lcd_i2c.init();
  lcd_i2c.backlight();
  lcd_i2c.setCursor(0,0);
  lcd_i2c.print("Hello");
  */
  //------------------------//
  //pinMode(PUSH_BUTTON1,INPUT);
  // -----------------------//
 // ledcAttachPin(PIN_SERVO, CANALDEFAULT);
  //ledcSetup(CANALDEFAULT, FREQDEFAULT, RESOLUCAO);

  //pinMode(PIN_POT,INPUT);
  // analogReadResolution(16);
  // -----------------------//
  

  //ubidots.setCallback(callback);
  //ubidots.setup();
  //ubidots.reconnect();

//  timer = millis();
  


}
void loop() {
 // int button1Pressed = digitalRead(PUSH_BUTTON1);
 // contador++;

//  potValue = analogRead(PIN_POT);
 // duty = potValue / 500 + 30;
  //Serial.printf("Potenciometro: %i\n",potValue);
  /*
  if(button1Pressed){
    if(buttonState){
      buttonState = false;
      // Faz algo
      duty = 30;
      lcd_i2c.setCursor(0,1);
      lcd_i2c.printf("Cnt %3i-Duty %3i",contador,duty);
      
  //    ledcWrite(CANALDEFAULT,duty);
    }
  }else{
    buttonState = true;
   // ledcWrite(CANALDEFAULT,duty);
  }
  lcd_i2c.setCursor(0,0);
  lcd_i2c.printf("Cnt %3i-Duty %3i",contador,duty);
  */
  delay(500);
   /*
  // ------------------------------
  if (!ubidots.connected())  {
    ubidots.reconnect();
    Serial.println("Reconectando");
  }
  if ( (millis() - timer) > PUBLISH_FREQUENCY) {
    float value = 666;
    ubidots.add(VARIABLE_LABEL, value); // Insert your variable Labels and the value to be sent
    ubidots.publish(DEVICE_LABEL);
    timer = millis();
  }
  ubidots.loop();
  */


 
}
