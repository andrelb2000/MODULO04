
/****************************************
 * Include Libraries and DEFINES
 ****************************************/
#include "WiFi.h"
#include "UbidotsEsp32Mqtt.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//#define I2C_SDA 21
//#define I2C_SCL 22
#define I2C_SDA 40
#define I2C_SCL 39
/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBUS-BfoSnxYqvATskbje5HR01TFwOx6F9e";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "...";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "...";      // Put here your Wi-Fi password

const char *DEVICE_LABEL = "2023_2b_m4_t7";   // Put here your Device label to which data  will be published
const char VAR_ALUNO_DADOS[255] = "alunotestXXX"; // Put here your Variable label to which data  will be published
const int PUBLISH_FREQUENCY = 240000; // Tempo em cada estação em Milisegundos
unsigned long timer;
char listaValorSinal[4048];
int   sinalAtelier1 = 0, sinalAtelier4  = 0, sinalR09      = 0, sinalLaboratorio = 0,
      sinalAtelier8 = 0, sinalAtelier12 = 0, sinalRecepcao = 0, sinalITBar       = 0;
int mainSignalValue = 0;
int loopCounter = 1;
int loopVarredura = 0;
int sequenciaAleatoria[10] = {0,1,2,3,4,5,6,7,8,9};
char localVarredura[255] = "INICIO";
Ubidots ubidots(UBIDOTS_TOKEN);
LiquidCrystal_I2C  lcd_i2c(0x27, 16, 2);



// Reorganiza aleatoriamente a sequência de visitação dos pontos 
void geraSeqAleatoria(){
  for (int i = 7; i > 0; i--) {
    int j = (rand() % (i + 1)) + 1;
    int temp = sequenciaAleatoria[i];
    sequenciaAleatoria[i] = sequenciaAleatoria[j];
    sequenciaAleatoria[j] = temp;
  }
}
//Passa para o próximo ponto de checagem
void avancaChecagem(){
    loopVarredura++;  
    loopVarredura = loopVarredura % 10;
    Serial.printf("Loop %i - Escolha %i \n",loopVarredura,sequenciaAleatoria[loopVarredura]);
    switch(sequenciaAleatoria[loopVarredura]){
      case 0:  strcpy(localVarredura,"INICIO");      break;
      case 1:  strcpy(localVarredura,"Atelier 1");   break;
      case 2:  strcpy(localVarredura,"Atelier 4");   break;
      case 3:  strcpy(localVarredura,"Atelier 8");   break;
      case 4:  strcpy(localVarredura,"Atelier 12");  break;
      case 5:  strcpy(localVarredura,"Sala R9");     break;
      case 6:  strcpy(localVarredura,"IT Bar");      break;
      case 7:  strcpy(localVarredura,"Laboratorio"); break;
      case 8:  strcpy(localVarredura,"Recepcao");    break;
      case 9:  strcpy(localVarredura,"FIM");         break;
    }
}
// Usa o valor atual de pontência de sinal e atribui ao ponto determinado
void atribuiSinal(){
    mainSignalValue = WiFi.RSSI();
    Serial.print("Sequencia: ");
    Serial.println(sequenciaAleatoria[loopVarredura]);
    Serial.print("loopVarredura: ");
    Serial.println(loopVarredura);
    switch(sequenciaAleatoria[loopVarredura]){
      case 1:  sinalAtelier1    = mainSignalValue;  break;
      case 2:  sinalAtelier4    = mainSignalValue;  break;
      case 3:  sinalAtelier8    = mainSignalValue;  break;
      case 4:  sinalAtelier12   = mainSignalValue;  break;
      case 5:  sinalR09         = mainSignalValue;  break;
      case 6:  sinalITBar       = mainSignalValue;  break;
      case 7:  sinalLaboratorio = mainSignalValue;  break;
      case 8:  sinalRecepcao    = mainSignalValue;  break;
    }
}

// Função de callback padrão - Não usada no momento
void callback(char *topic, byte *payload, unsigned int length){
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
  Serial.begin(115200);
  //------------------------//
  lcd_i2c.init();
  lcd_i2c.backlight();
  lcd_i2c.setCursor(0,0);
  lcd_i2c.print("Que os Jogos  ");
  lcd_i2c.setCursor(0,1);
  lcd_i2c.print("COMECEM!!!!");
  //------------------------//

  //ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  timer = millis()-PUBLISH_FREQUENCY;
  srand((unsigned int)millis());
  geraSeqAleatoria();
}

//////////////////////////////////////////
void loop() {
  int connectProb = 0;
  while (!ubidots.connected())  {
    Serial.print("Problemas e reconectando no inicio do LOOP - ");
    Serial.println(connectProb);    
    connectProb++;
    lcd_i2c.setCursor(0,0);
    lcd_i2c.print("Prob.Recon.LOOP");
    lcd_i2c.setCursor(0,1);
    lcd_i2c.printf("Tent.Reconectar");
    delay(100);
    ubidots.reconnect();
    lcd_i2c.setCursor(0,1);
    lcd_i2c.printf("Reconectando %2i",connectProb);
    delay(100); 
  }
  if(loopVarredura < 9 ){
    // Publica o valor atual do sinal no ponto determinado
    if ((millis() - timer) > PUBLISH_FREQUENCY){
      atribuiSinal();
      lcd_i2c.setCursor(0,0);
      lcd_i2c.print("CAPTURANDO SINAL");
      lcd_i2c.setCursor(0,1);
      lcd_i2c.printf("%-9s %4idb",localVarredura,mainSignalValue);
      //---------------------------------------//
      //      BOM LUGAR PARA UM BUZZER         //
      //---------------------------------------//
      sprintf(listaValorSinal," \"atelier1\":%i, \"atelier4\":%i, \"atelier8\":%i, \"atelier12\":%i,\"atelierR9\":%i,\"itBAR\":%i,\"laboratorio\":%i,\"recepcao\":%i",  
                              sinalAtelier1,sinalAtelier4,sinalAtelier8,sinalAtelier12,sinalR09,sinalITBar,sinalLaboratorio,sinalRecepcao);  
      ubidots.add(VAR_ALUNO_DADOS, mainSignalValue ,listaValorSinal);
      Serial.println(listaValorSinal);
      connectProb = 0;
      while (!ubidots.connected())  {        
        connectProb++;
        Serial.print("Problemas e reconectando antes da publicacao - ");
        Serial.println(connectProb);
        lcd_i2c.setCursor(0,0);
        lcd_i2c.print("Prob.Reconex.Pub");
        lcd_i2c.setCursor(0,1);
        lcd_i2c.printf("Tent.Reconectar");
        delay(100);
        ubidots.reconnect();
        lcd_i2c.setCursor(0,1);
        lcd_i2c.printf("Reconectando %2i",connectProb);
        delay(100);               
      }
      ubidots.publish(DEVICE_LABEL);
      delay(10000);
      timer = millis();
      avancaChecagem();
      Serial.print("Va para ");
      Serial.println(localVarredura);
      lcd_i2c.setCursor(0,0);
      lcd_i2c.printf("%1i-Va AGORA para:",loopVarredura);
      lcd_i2c.setCursor(0,1);
      lcd_i2c.printf("-> %-12s ",localVarredura);
      delay(5000);
    }else{
      if(  ((millis() - timer) % 100) == 0){
       lcd_i2c.setCursor(0,0);
      lcd_i2c.printf("Faltam %5.1f seg.", ((float)PUBLISH_FREQUENCY - (float)(millis() - timer))/1000.0  );
      }    
    }
  }else{
    mainSignalValue = WiFi.RSSI() + 70;
    sprintf(listaValorSinal," \"atelier1\":%f, \"atelier4\":%f, \"atelier8\":%f, \"atelier12\":%f,\"atelierR9\":%f,\"itBAR\":%f,\"laboratorio\":%f,\"recepcao\":%f",  
                              sinalAtelier1,sinalAtelier4,sinalAtelier8,sinalAtelier12,sinalR09,sinalITBar,sinalLaboratorio,sinalRecepcao);  
    ubidots.add(VAR_ALUNO_DADOS, mainSignalValue ,listaValorSinal);
    ubidots.publish(DEVICE_LABEL);
  }
  ubidots.loop();  
}
