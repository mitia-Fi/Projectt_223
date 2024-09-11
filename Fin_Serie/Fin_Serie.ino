char caractL = 0;
int nbcaractD = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}
void loop()
{
  nbcaractD = 'A';
  while(nbcaractD>0)
  {
    caractL = Serial.read();
    if(caractL == 'A')
    {
      digitalWrite(13, HIGH);
      Serial.println("Lampe Alumer");
    }
    if(caractL == 'M')
    {
      digitalWrite(13, LOW);
      Serial.println("Lampe Eteinte");
    }
    caractL =   caractL + 32; //Code Ascii
    Serial.println(caractL);
  }
}
