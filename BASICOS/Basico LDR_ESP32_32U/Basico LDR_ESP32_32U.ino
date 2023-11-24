#define PINLDR_DEF  1

int valorLDR = 0;
int minLDR = 600;
int maxLDR = 2400;
int normalLDR = 0;

void setup() {  
  Serial.begin(115200);
  pinMode(PINLDR_DEF,INPUT);             
}
void loop() {  
  valorLDR = analogRead(PINLDR_DEF);
  normalLDR = (int) 5* ( (float) valorLDR - (float)minLDR)/((float)maxLDR - (float)minLDR);
  Serial.printf("Valor LDR normalizado: %4i\n",normalLDR);
  delay(500);
}
