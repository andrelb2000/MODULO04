#define PERIODO_50HZ 20   // 20 ms
#define CANALDEFAULT  0   
#define FREQDEFAULT  50   // 1/(20ms)
#define RESOLUCAO 10      // 10 Bits = 2048
#define PIN_SERVO 27

int duty = 50;
//  0 ms   --->    0 
// 20 ms   ---> 2048  
//  1.0 ms ---> 2048/20 = ~100 --> -90 Graus
//  1.5 ms --->         = ~150 -->   0 Graus
//  2.0 ms --->         = ~200 --> -90 Graus

void setup() {
  Serial.begin(115200);
  ledcAttachPin(PIN_SERVO, CANALDEFAULT);
  ledcSetup(CANALDEFAULT, FREQDEFAULT, RESOLUCAO);
}
void loop() { 
  duty = 50;
  Serial.printf("Duty = %3i",duty);      
  ledcWrite(CANALDEFAULT,duty);
  delay(500);
 
  duty = 100;
  Serial.printf("Duty = %3i",duty);      
  ledcWrite(CANALDEFAULT,duty);
  delay(500);

  duty = 150;
  Serial.printf("Duty = %3i",duty);      
  ledcWrite(CANALDEFAULT,duty);
  delay(500);
}
