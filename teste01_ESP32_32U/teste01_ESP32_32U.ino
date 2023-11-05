int pino35out = 33;


void setup() {
  // put your setup code here, to run once:
  pinMode(pino35out,OUTPUT);
  digitalWrite(pino35out,HIGH);
  Serial.begin(115200);
  Serial.println("Iniciando a serial");

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(pino35out, LOW);
  Serial.println("Apagando");
  delay(500);
  digitalWrite(pino35out, HIGH);
  Serial.println("Acendendo");
  delay(500);
}
