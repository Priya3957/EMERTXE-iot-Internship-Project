#include "ldr.h"
#include "Arduino.h"
#include "main.h"

void init_ldr(void)
{
   pinMode(GARDEN_LIGHT, OUTPUT);
   
}
void brightness_control(void)
{
  unsigned int ldr_val;
  ldr_val=analogRead(LDR_SENSOR);
  ldr_val=(1023-ldr_val)/4;
  analogWrite(GARDEN_LIGHT,ldr_val);
  delay(100);

}
