#include "module.h"
#include "Adafruit_MLX90614.h"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
double emissivity = 0.95;

int setup_thermo() {
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.clear(BLACK);

    mlx.begin();
    int ret = mlx.begin();
    //mlx.writeEmissivity(emissivity);
    return ret;
}


void loop_thermo() {
    // surface temp
    Serial.print("Temperature : ");  Serial.println(mlx.readObjectTempC());
    // ambient temp
    Serial.print("Ambient : ");  Serial.println(mlx.readAmbientTempC());
    Serial.print("emmisivity : ");  Serial.println(mlx.readEmissivity());
    Serial.println();

    M5.Lcd.setCursor(50, 100);
    M5.Lcd.print(mlx.readObjectTempC());
    M5.Lcd.print(" degree");

    read_batt();
    delay(500);
    M5.Lcd.clear(BLACK);
}

