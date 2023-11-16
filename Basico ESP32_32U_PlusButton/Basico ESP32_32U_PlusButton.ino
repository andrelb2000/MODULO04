#define PUSH_BUTTON1 12
int buttonState = true;
int contador = 0;
void setup() {
  Serial.begin(115200);
  pinMode(PUSH_BUTTON1,INPUT);
}
void loop() {
  int button1Pressed = digitalRead(PUSH_BUTTON1);
  contador++;

  if(button1Pressed){
    if(buttonState){
      buttonState = false;
      // Faz algo quando o botão é pressionado UMA vez
      // e só faz de novo quando o botão é liberado 
      // e pressionado de novo
      Serial.printf("PRESSIONADO %i",contador);
    }
  }else{
    buttonState = true;
  }  
  Serial.printf("CONTANDO %i",contador);
  delay(500); 
}
