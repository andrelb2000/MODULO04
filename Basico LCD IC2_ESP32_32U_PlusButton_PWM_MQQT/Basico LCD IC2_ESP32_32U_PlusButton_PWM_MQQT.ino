



//#include <Wire.h>


////#define I2C_SDA 21
////#define I2C_SCL 22




/****************************************
 * Define Constants
 ****************************************/
                     
//const char *WIFI_SSID = "Inteli-COLLEGE";      // Put here your Wi-Fi SSID
//const char *WIFI_PASS = "QazWsx@123";      // Put here your Wi-Fi password

const char *WIFI_SSID = "AMM_NOVOS_HORIZONTES_5G";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "melissabraga";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "ALB_NOTE001";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "ALB_DUTY001"; // Put here your Variable label to which data  will be published
const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds




void setup() {
  Serial.begin(115200);
}
void loop() {
  Serial.print(".");
  delay(1000); 
}
