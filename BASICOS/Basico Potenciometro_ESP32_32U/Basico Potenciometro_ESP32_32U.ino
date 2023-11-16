#define PIN_POT 34
int potValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_POT,INPUT);
  analogReadResolution(16);
}
void loop() {
  potValue = analogRead(PIN_POT);
  Serial.printf("Potenciometro: %i\n",potValue);  
  delay(500); 
}
