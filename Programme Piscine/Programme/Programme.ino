#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Servo.h> +
#include <EEPROM.h> 
#include <ezBuzzer.h>
//#include "pitches.h"

Servo MonServo; //On attribue au MonServo le variable de contrôle du servo
SoftwareSerial MonGSM(12,11); //Pin Arduino : 11 rélier a TX du GSM et 12 réliser au RX
 
int positionS = 0;  
int inter = 0;
const int pin10 = 10;
const int pin8 = 8;
const int LIGNES = 4;
const int COLONNES = 4;
const int ledR = 13;
const int ledV = 7;
String recuMessage;
bool statusS = false;
bool etat = false;
const String password; //Le mot de passe dans EEPROM
char key; //La Variable qui vas stocker les valeurs des touche appuyée 
String entrerPassword = ""; //On initialise la variables qui stock les incrémentations de touche de key
unsigned long initTime = millis(); //On utilise millis au lieu de delay() car, delay est bloquante
int attente = 250; 
const int C4 = 2; //Broche numérique keypad en sortie (1P keypad -> D2 Arduino)
const int C3 = 3; //Broche numérique keypad en sortie (2P keypad -> D3 Arduino)
const int C2 = 4; //Broche numérique keypad en sortie (3P keypad -> D4 Arduino)
const int C1 = 5; //Broche numérique keypad en sortie (4P keypad -> D5 Arduino)
const int L4 = 16; //Broche analogique keypad en entrée (5P keypad -> A2 Arduino)
const int L3 = 17; //Broche analogique keypad en entrée (6P keypad -> A3 Arduino)
const int L2 = 18; //Broche analogique keypad en entrée (7P keypad -> A4 Arduino)
const int L1 = 19; //Broche analogique keypad en entrée (8P keypad -> A5 Arduino)
const int Debut = 90; 
const int Fin = -90;

char touches[LIGNES][COLONNES] = 
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
/*
const int Anal5 = A5;
const int Anal5 = A4;
const int Anal5 = A3;
const int Anal5 = A2;

  int A = analogRead(A5);
  int B = analogRead(A2);
  int C = analogRead(A3);
  int D = analogRead(A2);
  byte pinLignes[LIGNES] = {9, 8, 7, 6};
  byte pinColonnes[COLONNES] = {A, B, C, d};
*/

byte pinLignes[LIGNES] = {L1,L2,L3,L4};
byte pinColonnes[COLONNES] = {C1,C2,C3,C4}; //On utilise les pines analogique

Keypad clavier = Keypad(makeKeymap(touches), pinLignes, pinColonnes, LIGNES, COLONNES); 
ezBuzzer buzzer(pin8);


void setup()
{
  if((millis() - initTime) > attente)
  {
    etat = !etat;
    initTime = millis();
  }
  MonServo.attach(pin10);
  MonGSM.begin(19200);
  delay(1000);
  Serial.begin(19200);
  Serial.println("Le GSM est maintenant Operationnel ! ");
  entrerPassword.reserve(6);
  
  pinMode(13, OUTPUT); //Pour led Rouge
  pinMode(12, OUTPUT); //Pour led Verte

  digitalWrite(ledR, HIGH);
}
  
void loop()
{ 
  buzzer.loop();
  key = clavier.getKey(); //Lecture de la touche appuyée
    
  if(key)
  {
    Serial.println(key);
    delay(50);
    if (key == '*')
    {
      entrerPassword = "";
      Serial.println("Mot de passe effacer");
    }
    else if(key == '#')
    {
      if(entrerPassword.length()>0)
      {
        entrerPassword.remove(entrerPassword.length()-1, 1);
      }
      delay(300);
    }
    else if (key == 'A')
    {
      entrerPassword = "";
      fermer();
    }
    else if(key == 'B')
    {
      entrerPassword = ""; 
      Serial.println("Base de donner");//A CHERCHER LA LIAISON DE BASE DE DONNER ET ARDUINO
    }
    else if(key == 'C') //Pour alumer le lampe extérieur
    {
      statusS = !statusS; 
      if(statusS == false)
      {
        Serial.println("Lampe exterieur Eteinte");
      }
      else
      {
        Serial.println("Lampe exterieur Alumer");
      }  
    }
    else if(key =='D')
    { 
      //RECUPERE LES NUM2RO DANS LA BASE DE DONNER
      numero("+261342020035");
      numero("+261346850871");
      numero("+261346720633");
      //delay(100);
      /*
      numero("+261346720633");
      numero("+261340731748");
      numero("+261345241893");
      */
      delay(200);
      entrerPassword = "";
    }
    else
    {
      entrerPassword += key;
      Serial.println("Code : "+entrerPassword + "\n");
      delay(300); //Pause entrer 2 appuis
    }   
  }
  else if(MonGSM.available()) //Controler la maison A distance
  {
    recuMessage = MonGSM.readStringUntil('\n');
    Serial.println(recuMessage);
    delay(5000);
    reception();
    delay(100);
    /*
     if(recuMessage.indexOf("Fermer La Porte")>0)
     {
        ouvrir();
     }
     */
    
    if(recuMessage == "Fermer la Porte")
    {
      if(positionS != Debut)
      {
        fermer();
        delay(500);
        MonGSM.println("Bravo, la porte est Fermer");
        delay(100);
      }
      else
      {
        MonGSM.println("La porte est déja Fermer");
        delay(100);
      }
    }
    else if(recuMessage == "Ouvrer la Porte")
    {
      if(positionS != 0)
      {
        ouvrir();
        delay(500);
        MonGSM.println("Bravo, la porte est Ouverte"); 
        delay(100);
      }
      else
      {
        MonGSM.println("La porte est déja ouvert");
        delay(100);
      }
    }
    else if(recuMessage == "Urgence")
    {
      allarmeUrgence();
      delay(500);
      cirene();
    }
    else if(recuMessage == "Feu")
    {
      allarmeFeu();
      delay(500);
      ouvrir();
      delay(500);
      cirene();
    }
    else
    {
      MonGSM.println("Commande Introuvable, réessayer");
      delay(100); 
    }
    
    /*
    Serial.write(MonGSM.read());
    MonGSM.println("AT+CMGR = \"19\"");
    MonGSM.println('Le message est bien envoyer');
    MonGSM.println("AT+CMGD = \"19\"");
    */
  }
}

String motDePasse() //Fonction pour enregistrer le mot de passe dans le EEPROM
{
  password = "123456";
  int numMemoire = 300;
  if(EEPROM.read(numMemoire) != password.toInt()) //On vériifier si le mot de passe est déja inscrite dans le mémoire EEPROM, si non on le créer
  {
    EEPROM.put(numMemoire,password);
    Serial.println("Dans le " + EEPROM.put(numMemoire,password));
  }
  return EEPROM.get(numMemoire,password);
}

void numero(String entrer) //Fonction nvoyer de sms via GSM
{
  MonGSM.println("AT+CMGF = 1\r");
  delay(1000);
  MonGSM.println("AT+CMGS =\""+entrer+"\"");
  delay(1000);

  verification();
  
  MonGSM.println((char)26);
  delay(1000);
  MonGSM.println();
  delay(5000); 
}

void reception() //Fonction pour la reception de MSG de Destinataire vers GSM
{
  Serial.println("AT+CMGF = 1");
  delay(1000);
  Serial.println("AT+CNMI = 2, 2, 0, 0\r");
  delay(1000);
}

void fermer() //Fonction pour fermer la porte
{
  entrerPassword = "";
  MonServo.write(Fin);
  delay(50);
/*  
  if (positionS < 0)
  {
    for (positionS = 90; positionS >=0; positionS -= 0.5)
    {
      MonServo.write(positionS);
      delay(15);
    }
    positionS = 0;
  }
*/
}

void ouvrir() //Fonction pour ouvrir la porte:
{
  MonServo.write(Debut);
  delay(100);
  /* 
  for (positionS = 0; positionS <=90; positionS -= 0.5)
  {
    MonServo.write(positionS);
    delay(50);
  } 
  */
}

void allumerLedR() //Fonction pour allumer le led rouge
{
  digitalWrite(ledR, HIGH);
  delay(100);
  digitalWrite(ledV, LOW);
}

void allumerLedV() //Fonction pour allumer le led verte
{
  digitalWrite(ledR, LOW);
  delay(100);
  digitalWrite(ledV, HIGH);
}


void cirene() //Fonction pour cirene d'alarme
{
  for(int i = 0; i < 10; i++)
  {
    ezBuzzer(2500); //Appel de la fonction buzzer
    digitalWrite(ledR, etat);
    delay(500);
    digitalWrite(ledR, etat);
    delay(500);
  }
}

void allarmeUrgence() //Fonction pour appeler un Ambulance
{
  MonGSM.println("Urgence Médical, nous avons contacter un Ambilance");
  numero("+261387265705");
}

void allarmeFeu() //Fonction pour pour appeler les pompiers
{
  MonGSM.println("Alert feu, nous avons contacter les Pompiers");
  numero("+261387265705");
}

void verification() //Fonction pour vérifier le mot de passe:
{
  //Serial.println(motDePasse());
  String intermediaire = String(motDePasse());
  if( intermediaire != String(entrerPassword) ) //IL FAUT AUSSI VERIFIER SI LE MOT DE PASSE EST PRESENT DANS LA BASE DE DONNER
  { 
    MonGSM.println("Mot de passe non correct, reessayer !!");
    Serial.println("Message erreur envoyer");
    delay(500);
    fermer();
    delay(500);
    cirene(); 
    delay(500);
    inter = 0;
  }
  else
  {
    MonGSM.println("Mot de passe correct, bonjour Monsieur");
    Serial.println("Message acces envoyer");
    delay(300);
    ouvrir();
    delay(100);
    allumerLedV(); 
  }
}
