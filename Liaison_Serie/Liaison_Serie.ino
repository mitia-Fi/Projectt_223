char i = 0;

void setup()
{
  Serial.begin(9600);  
}
void loop()
{
  char lettre = 'A';
  for (i = 0; i < 26; i++)
  {
    Serial.println(lettre);
    lettre = lettre + 1;
    delay(250); 
  }
  Serial.println(""); //Rétour a la ligne
  delay(5000);
}
