#define ECHODEFAULT  17
#define TRIGDEFAULT  18
//////////////////////////////
//////////////////////////////
int medir(){        
  long duration;        
  float distanceCm = 0;
  float SOUND_SPEED = 0.034;
  int tempoEnvio = 10;
  int tempoLimpeza = 2;

  // Clears the trigPin
  digitalWrite(TRIGDEFAULT, LOW);
  delayMicroseconds(tempoLimpeza);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIGDEFAULT, HIGH);
  delayMicroseconds(tempoEnvio);
  digitalWrite(TRIGDEFAULT, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHODEFAULT, HIGH);
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2; 
  return(distanceCm);
};

void setup() {  
  Serial.begin(115200);
  pinMode(TRIGDEFAULT,OUTPUT);
  pinMode(ECHODEFAULT,INPUT);
}

void loop() {
  float distancia = medir();
  Serial.println("Medindo:");
  Serial.printf("Distancia %4f cm\n",distancia);
}
