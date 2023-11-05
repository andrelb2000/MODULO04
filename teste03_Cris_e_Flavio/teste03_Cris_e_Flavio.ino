const int pin25out = 25;
void setup() {
  // Define o pino 25 como SAIDA
  pinMode(pin25out,OUTPUT);
  // Inicializa a porta serial de com. com o ESP32
  Serial.begin(115200);
  Serial.println("Iniciando a serial");
}
void loop() {
  // Envia 0V. para o pino 25
  digitalWrite(pin25out, LOW);
  Serial.println("Apagando");
  // Espera 1 seg.
  delay(1000);
  // Envia 3.3V. para o pino 25
  digitalWrite(pin25out, HIGH);
  Serial.println("Acendendo");
  // Espera 1 seg.
  delay(1000);
}
