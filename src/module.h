#include <M5Stack.h>
#include <Wire.h>
#include "env_sensor/env.h"
#include "thermo_sensor/thermo.h"
#include "dist.h"

int setup_dist();
void loop_dist();
int setup_thermo();
void loop_thermo();
void udp_setup();
void udp_loop(String val);

void sensor_error();
void read_batt();

