/*************************************************************
Title         :   Home automation using blynk
Description   :   To control light's brigntness with brightness,monitor temperature , monitor water level in the tank through blynk app
Pheripherals  :   Arduino UNO , Temperature system, LED, LDR module, Serial Tank, Blynk cloud, Blynk App.
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPL3j8MvtKaw"
#define BLYNK_TEMPLATE_NAME "HOME AUTOMATION"
#define BLYNK_AUTH_TOKEN "Kh6MROPyq9Bk9tzKjhKGHFDrSdkLAL_b"

// Comment this out to disable prints 
//#define BLYNK_PRINT Serial

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>*/

#include "main.h"
#include "temperature_system.h"
#include "ldr.h"
#include "serial_tank.h"

char auth[] = BLYNK_AUTH_TOKEN;
bool heater_sw,cooler_sw;
bool inlet_sw,outlet_sw;
unsigned int tank_volume;

BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// This function is called every time the Virtual Pin 0 state changes
/*To turn ON and OFF cooler based virtual PIN value*/
BLYNK_WRITE(COOLER_V_PIN)
{
  cooler_sw= param.asInt();
  if(cooler_sw)
  {
      cooler_control(ON);  
       lcd.setCursor(7,0);
     lcd.print("CO_LR ON  ");  
  }
  else
  {
    cooler_control(OFF);
    lcd.setCursor(7,0);
    lcd.print("CO_LR OFF");
  }
  
}
//To turn ON and OFF heater based virtual PIN value
BLYNK_WRITE(HEATER_V_PIN )
{
  heater_sw= param.asInt();

  if(heater_sw)
  {
    heater_control(ON);  
    lcd.setCursor(7,0);
    lcd.print("HT_R ON  ");  
  }
  else
  {
    heater_control(OFF);
    lcd.setCursor(7,0);
    lcd.print("HT_R OFF");
  }
  
}
//To turn ON and OFF inlet vale based virtual PIN value
BLYNK_WRITE(INLET_V_PIN)
{
  inlet_sw= param.asInt();
  if(inlet_sw)
  {
     enable_inlet();
     lcd.setCursor(7,1);
    lcd.print("IN_FL_ON  ");
  }
  else
  {
     disable_inlet();
     lcd.setCursor(7,1);
    lcd.print("IN_FL_OFF");
  }
}
//To turn ON and OFF outlet value based virtual switch value
BLYNK_WRITE(OUTLET_V_PIN)
{
  outlet_sw= param.asInt();
  if(outlet_sw)
  {
     enable_outlet();
     lcd.setCursor(7,1);
    lcd.print("OT_FL_ON  ");
  }
  else
  {
     disable_outlet();
    lcd.setCursor(7,1);
    lcd.print("OT_FL_OFF");
  }
}
//To display temperature and water volume as gauge on the Blynk App*/
void update_temperature_reading()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(TEMPERATURE_GAUGE,read_temperature());
  Blynk.virtualWrite(WATER_VOL_GAUGE,volume());
  
}

//To turn off the heater if the temperature raises above 35 deg C
void handle_temp(void)
{
  if((read_temperature() > float(35)) && heater_sw)
  {
    heater_sw=0;
    heater_control(OFF);
    lcd.setCursor(7,0);
    lcd.print("HT_R OFF");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN,"Temerature is above 35 deg celcius \n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN,"Turning off the heater\n");
    Blynk.virtualWrite(HEATER_V_PIN,0);
  }
}

//To control water volume above 2000ltrs
void handle_tank(void)
{
  if((tank_volume < 2000)&&(inlet_sw == 0))
  {
    enable_inlet();
    inlet_sw=1;
    lcd.setCursor(7,1);
    lcd.print("IN_FL_ON  ");
     Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN,"Volume of water in the tank is less than 2000 ltr \n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN,"Turning on the inlet valve\n");
    Blynk.virtualWrite(INLET_V_PIN,1);
    
  }
  if((tank_volume == 3000)&&(inlet_sw == 1))
  {
    disable_inlet();
    inlet_sw=0;
    lcd.setCursor(7,1);
    lcd.print("IN_FL_Off");
     Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN,"tank is full\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN,"Turning of the ilet valve\n");
    Blynk.virtualWrite(INLET_V_PIN,1);
    
  }
  

}


void setup(void)
{
  init_ldr();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("T=");
  Blynk.begin(auth);
  lcd.setCursor(0,1);
  lcd.print("V=");
  init_temperature_system();
   init_serial_tank();
  timer.setInterval(500L,update_temperature_reading);

}

void loop(void) 
{
     Blynk.run();
     timer.run();
     brightness_control();
     String temperature;
     temperature= String(read_temperature(),2);
     lcd.setCursor(2,0);
     lcd.print(temperature);
     tank_volume = volume();
     lcd.setCursor(2,1);
     lcd.print(tank_volume);
     handle_temp();
     handle_tank();
}
