
/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsEsp32Mqtt.h"

/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBUS-Omv5uCEHXbcyX6yicsOBgzZG3AMgOh";  // Put here your Ubidots TOKEN
//const char *WIFI_SSID = "AndroidAPEED8";      // Put here your Wi-Fi SSID
//const char *WIFI_PASS = "spiv092312345";      // Put here your Wi-Fi password
const char *WIFI_SSID = "AMM_NOVOS_HORIZONTES_2G";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "melissabraga";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "esp32test";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL1 = "VARLABEL001"; // Put here your Variable label to which data  will be published
const char *VARIABLE_LABEL2 = "VARLABEL002"; // Put here your Variable label to which data  will be published
const char *VARIABLE_LABEL3 = "VARLABEL003"; // Put here your Variable label to which data  will be published



const int PUBLISH_FREQUENCY = 10000; // Update rate in milliseconds

unsigned long timer;
char *contextoM1 = "{ \"ponto_medida\" : \"ATELIER 1\"}";


Ubidots ubidots(UBIDOTS_TOKEN);

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  Serial.println("Apos a conexao");
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  timer = millis();

}
int value1 = 1;
int value2 = 1;
int value3 = 1;

void loop() {
  // put your main code here, to run repeatedly:
  if (!ubidots.connected())  {
    ubidots.reconnect();
  }
  if ((millis() - timer) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {
    ubidots.add(VARIABLE_LABEL1, value1%200,contextoM1); // Insert your variable Labels and the value to be sent
    ubidots.add(VARIABLE_LABEL2, value2%250 ); // Insert your variable Labels and the value to be sent
    ubidots.add(VARIABLE_LABEL3, value3%350 ); // Insert your variable Labels and the value to be sent
   
    
    ubidots.publish(DEVICE_LABEL);
    timer = millis();
  }
  value1+=1;  value2+=5;  value3+=20;
  ubidots.loop();
  Serial.print(".");
}
