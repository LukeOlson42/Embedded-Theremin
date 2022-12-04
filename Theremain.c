#include "inc/System.h"
#include "inc/LCD.h"


void ThereminInit(void);

int main(void)
{

    ThereminInit();

    for(;;)
    {

        // Output pitch on speaker - depends on volume, key, temperature

        // update LCD

        // update seven-segment

        // update eeprom if needed




    }


    return 0;
}

void ThereminInit(void)
{
    LCDInit();
    
}