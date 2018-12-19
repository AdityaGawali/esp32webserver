#include <driver/dac.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

void writeVoltage(dac_channel_t channel ,float  volt)
{
    if(volt >= 0 || volt <= 3.3)
    {
    int  value;
    value = (volt*255)/3.3;
    dac_output_voltage(channel , value);
     }

}

void app_main()
{

    dac_output_enable(DAC_CHANNEL_1);
    while(1)
    {
    writeVoltage(DAC_CHANNEL_1 ,2.42);
    }
 
}