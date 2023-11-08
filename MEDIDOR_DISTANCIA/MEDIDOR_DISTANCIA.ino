#define ECHODEFAULT  17
#define TRIGDEFAULT  18
//////////////////////////////
//////////////////////////////
class MedidorDistancia{
  private:
    int pinEcho  = ECHODEFAULT;
    int pinTrigg = TRIGDEFAULT;
    float SOUND_SPEED = 0.034;
    float distanceCm = 0;
    int tempoEnvio = 10;
    int tempoLimpeza = 2;
  public:
    MedidorDistancia(int t=10,int echo = ECHODEFAULT, int trig = TRIGDEFAULT){
      pinEcho = echo;
      pinTrigg = trig;
      tempoEnvio = t;
      pinMode(pinTrigg,OUTPUT);
      pinMode(pinEcho,INPUT);
    };    
    int medir(){        
        long duration;        
        // Clears the trigPin
        digitalWrite(pinTrigg, LOW);
        delayMicroseconds(tempoLimpeza);
        // Sets the trigPin on HIGH state for 10 micro seconds
        digitalWrite(pinTrigg, HIGH);
        delayMicroseconds(tempoEnvio);
        digitalWrite(pinTrigg, LOW);
        // Reads the echoPin, returns the sound wave travel time in microseconds
        duration = pulseIn(pinEcho, HIGH);
        // Calculate the distance
        distanceCm = duration * SOUND_SPEED/2; 
        return(distanceCm);
    };
    int distanciaCm(){
        return(distanceCm);
    };
};
MedidorDistancia *md = NULL;
void setup() {
  Serial.begin(115200);
  //------------------------//
  md  = new MedidorDistancia();
  //------------------------//
}
void loop() {
  md->medir();
  Serial.println("Medindo:");
  Serial.printf("Distancia %4i cm\n",md->distanciaCm());
}
