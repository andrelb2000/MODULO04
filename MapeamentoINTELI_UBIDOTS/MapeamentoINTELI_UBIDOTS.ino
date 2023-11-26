
/****************************************
 * Include Libraries
 ****************************************/
#include "WiFi.h"
#include "UbidotsEsp32Mqtt.h"
/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBFF-L2UWDy9jLghHCxu8o0xL10OjOrWxcM";  // Put here your Ubidots TOKEN
//const char *WIFI_SSID = "AndroidAPEED8";      // Put here your Wi-Fi SSID
//const char *WIFI_PASS = "spiv092312345";      // Put here your Wi-Fi password
const char *WIFI_SSID = "AMM_NOVOS_HORIZONTES_2G";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "melissabraga";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "2023_2b_m4_t10";   // Put here your Device label to which data  will be published
const char VAR_ALUNO_DADOS[255] = "alunotest001"; // Put here your Variable label to which data  will be published
const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds
unsigned long timer;
char listaValorSinal[4048];
float sinalAtelier1 = 0, sinalAtelier4  = 0, sinalR09      = 0, sinalLaboratorio = 0,
      sinalAtelier8 = 0, sinalAtelier12 = 0, sinalRecepcao = 0, sinalITBar       = 0;
int mainSignalValue = 1;
int loopCounter = 1;
int loopVarredura = 0;
int sequenciaAleatoria[10] = {0,1,2,3,4,5,6,7,8,9};
char localVarredura[255] = "INICIO";
Ubidots ubidots(UBIDOTS_TOKEN);

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
    mainSignalValue = WiFi.RSSI() + 70;
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
  //ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  timer = millis();
  geraSeqAleatoria();
}
//////////////////////////////////////////
void loop() {
  if (!ubidots.connected())  {
    ubidots.reconnect();
  }
  if(loopVarredura < 9 ){
    // Publica o valor atual do sinal no ponto determinado
    if ((millis() - timer) > PUBLISH_FREQUENCY){
      atribuiSinal();
      sprintf(listaValorSinal," \"atelier1\":%f, \"atelier4\":%f, \"atelier8\":%f, \"atelier12\":%f,\"atelierR9\":%f,\"itBAR\":%f,\"laboratorio\":%f,\"recepcao\":%f",  
                              sinalAtelier1,sinalAtelier4,sinalAtelier8,sinalAtelier12,sinalR09,sinalITBar,sinalLaboratorio,sinalRecepcao);  
      ubidots.add(VAR_ALUNO_DADOS, mainSignalValue ,listaValorSinal);
      ubidots.publish(DEVICE_LABEL);
      timer = millis();
      avancaChecagem();
      Serial.print("Va para ");
      Serial.println(localVarredura);
    }else{
      if(  ((millis() - timer) % 1000) == 0){
       // Serial.printf("Faltam %3.3f seg.\n", ((float)PUBLISH_FREQUENCY - (float)(millis() - timer))/1000.0  );
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
