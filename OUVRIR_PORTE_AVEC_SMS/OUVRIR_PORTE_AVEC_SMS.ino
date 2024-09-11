//Pins utilisé :
byte pinLed1 = 2;
byte pinLed2 = 3;

bool etat = false;

//Temps Initial:
unsigned long initTimme = millis();

int attente = 250; //Durée de clignotement

String instruction = "";

void setup()
{
  Serial.begin(9600);

  pinMode(pinLed1, OUTPUT);
  pinMode(pinLed2, OUTPUT);
}
void loop()
{
  //Mise a jour de la variable etat
  if((millis() - initTime > attente)
  {
    etat = !etat; //Cela permet de changer l'etat de la led
    initTime = millis();
  }

  //Reception des données via la port série
  if(Serial.available()>0)
  {
    instruction = Serial.readStringUntil('\n'); //La valeur sera stocker sour forme de string
  }
  if(instruction =="all on"
  {
    digitalWrite(pinLed1, HIGH);
    digitalWrite(pinLed2, LOW);
  }
  else if(instruction == "led 1 on")
  {
    digitalWrite(pinLed1, LOW);
    digitalWrite(pinLed2, HIGH);
  }
  else if(instruction == "led 2 on")
  {
    digitalWrite(pinLed1, HIGH);
    digitalWrite(pinLed2, LOW);
  }
  else
  {
    digitalWrite(pinLed1, LOW);
    digitalWrite(pinLed2, LOW);
  }
  
}
