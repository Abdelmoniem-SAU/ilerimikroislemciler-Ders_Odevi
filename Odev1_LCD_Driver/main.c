#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "lcd.h"
//------------------------------------------------------------
// ÖDEV 1: 16x2 LCD Sürücü
//------------------------------------------------------------

int main(void)
{
    // Sistem saatini 50 MHz'e ayarla
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL |
                   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // LCD başlatma
    baslangic();
    LCD_sil();

    // 1. satıra metin yazdır
    satir_sutun(1, 0);
    printf("ABDELMONEIM");

    // 2. satıra metin yazdır
    satir_sutun(2, 0);
    printf("MIKRO1");

    while (1)
    {
        // Sonsuz döngü - LCD sabit yazı gösterir
    }
}
