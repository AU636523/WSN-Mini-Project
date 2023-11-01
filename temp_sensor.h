#pragma once

#define temp_sensor_d1 -40.1
#define temp_sensor_d2 0.01
#define SHT11_RAW_VALUE_TO_CELCIUS(v) (float)(temp_sensor_d1 + temp_sensor_d2*(float)v)