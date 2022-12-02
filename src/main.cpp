#include "module.h"

int dev_id = 9;

void sensor_error(){
  M5.Lcd.setTextColor(RED);
  M5.lcd.setCursor(50, 110);
  M5.lcd.fillScreen(BLACK);
  M5.Lcd.print("sensor error");
  M5.Lcd.setTextColor(WHITE);
}

void read_batt()
{
  int bat_level = M5.Power.getBatteryLevel();
    if (bat_level <= 25){
            M5.Lcd.setTextColor(RED);
        }
  M5.Lcd.setCursor(85, 210);
  M5.Lcd.printf("batt : %3d %%",bat_level);
  M5.Lcd.setTextColor(WHITE);
}

void setup(void)
{
  M5.begin();             // Init M5Stack
  M5.Power.begin();       // Init power  
  Wire.begin();           // Wire init, adding the I2C bus.
  Serial.begin(115200);

  if (setup_dist() == 1){
    dev_id = 1;
  } else if (setup_env() == 1){
    dev_id = 2;
  } else if (setup_thermo() == 1){
    dev_id = 3;
  }
  else{
    while(1){
      sensor_error();
      delay(1000);
    }
  }
}

void loop(void)
{
  if(dev_id == 1){
    loop_dist();
  } else if(dev_id == 2){
    loop_env();
  } else if(dev_id == 3){
    loop_thermo();
  }
}
