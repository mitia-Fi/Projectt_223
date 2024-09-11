#include <EEPROM.h>

/*
EEPROM.write() : pour écriture dans l'eeprom
EEPROM.update() : mise à jour dans l'eeprom
EEPROM.read() : lecture dans l'eeprom
  DEPUIS EEPROM ARDUINO UNO 1.62/
    * EEPROM.get() : remplace EEPROM.read()
    * EEPROM.put() : remplace EEPROM.write() et EEPROM.update()
EEPROM[addresse] : permet d'utiliser l'identifiant EEPROM comme dun tableau
*/

int code = "1234";

void setup()
{
   Serial.begin(19200);
}

void loop()
{
  
}

String enregistrement(String code)
{
  int nombres = 265;
  if(EEPROM.read(nombres) != code.toInt())
  {
    EEPROM.put(nombres, code);
  }
  return EEPROM.get(nombres, code);
}
