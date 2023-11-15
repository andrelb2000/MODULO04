#define PINLED1_DEF 4
#define PINLED2_DEF 5
#define PINLED3_DEF 6
#define PINLED4_DEF 7
#define PINLDR_DEF  1

class SensorLuz{
  private:
    int pinLED1 = PINLED1_DEF;
    int pinLED2 = PINLED2_DEF;
    int pinLED3 = PINLED3_DEF;
    int pinLED4 = PINLED4_DEF;
    int pinLDR = PINLDR_DEF;
    int valorLDR = 0;
    int minLDR = 600;
    int maxLDR = 2400;
    int normalLDR = 0;
    void pisca(int led){
      digitalWrite(pinLED1,LOW);
      digitalWrite(pinLED2,LOW);
      digitalWrite(pinLED3,LOW);
      digitalWrite(pinLED4,LOW);
      switch(led){
        case 0: digitalWrite(pinLED1,HIGH); break;
        case 1: digitalWrite(pinLED2,HIGH); break;
        case 2: digitalWrite(pinLED3,HIGH); break;
        case 3: digitalWrite(pinLED4,HIGH); break;
      }
    }
  public:
    SensorLuz(int l1 = PINLED1_DEF,int l2=PINLED2_DEF,
              int l3 = PINLED3_DEF, int l4=PINLED4_DEF, int ldr=PINLDR_DEF){
        pinLED1 = l1;
        pinLED2 = l2;
        pinLED3 = l3;
        pinLED4 = l4;
        pinLDR = ldr; 
        pinMode(pinLED1,OUTPUT);
        pinMode(pinLED2,OUTPUT);
        pinMode(pinLED3,OUTPUT);
        pinMode(pinLED4,OUTPUT);
        pinMode(pinLDR,INPUT);             
    }
    int acendeLEDLDR(){
      valorLDR = analogRead(pinLDR);
      normalLDR = (int) 5* ( (float) valorLDR - (float)minLDR)/((float)maxLDR - (float)minLDR);
      pisca(normalLDR);
      return(valorLDR);
    }
};
SensorLuz *sLuz = NULL;

void setup() {
  // put your setup code here, to run once:
  sLuz = new SensorLuz();
  Serial.begin(115200);
}
void loop() {
  int valor = sLuz->acendeLEDLDR();
  Serial.printf("LDR: %4i\n",valor);
  delay(500);

}
