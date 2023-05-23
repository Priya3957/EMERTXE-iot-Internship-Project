#include "temperature_system.h"
#include "Arduino.h"
#include "main.h"


void init_temperature_system(void)
{
    pinMode(HEATER,OUTPUT);
    pinMode(COOLER,OUTPUT);
    digitalWrite(HEATER,LOW);
    digitalWrite(COOLER,LOW);
}

float read_temperature(void)
{
   float temperature;
   temperature=(((analogRead(TEMPERATURE_SENSOR )*(float)5/1024))/(float)0.01);
   return temperature;
   
}

void cooler_control(bool control)
{
  if(control)
{
  digitalWrite(COOLER,HIGH);
}
else
{
  digitalWrite(COOLER,LOW);
}
   
}
void heater_control(bool control)
{
if(control)
{
  digitalWrite(HEATER,HIGH);
}
else
{
  digitalWrite(HEATER,LOW);
}
}
