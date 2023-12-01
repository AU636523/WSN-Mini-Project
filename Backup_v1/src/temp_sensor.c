#include "temp_sensor.h"
#include "src/ring_buffer.h"
#include "src/debug.h"

//Private Variables
static ring_buffer_t mbuf; //Storage of temperatures as raw UINT16 values (see temp_sensor.h for macro for conversion)

void temp_sensor_init(struct etimer* t)
{
    //ring buffer
    mbuf.capacity = NUM_MEASUREMENTS;
    
    //Initialize timer
    etimer_set(t, CLOCK_SECOND);

    //Activate Sensor
    SENSORS_ACTIVATE(sht11_sensor);

    DEBUG_MSG("temp_sensor: Measurement Process Started\n");
    
}

uint16_t get_sht11_value()
{
    return sht11_sensor.value(SHT11_SENSOR_TEMP);
}

bool temp_sensor_save_measurement()
{
    uint16_t v = get_sht11_value();
    DEBUG_MSG_INT("temp_sensor: Temp %d\n", v)

    bool s = ring_buffer_write(&mbuf, v);
    DEBUG_MSG(s ? "temp_sensor: Temp saved successfully\n" : "temp_sensor: Buffer full!\n")
    return s;
}