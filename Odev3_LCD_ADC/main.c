#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#include "lcd.h"

/*
 * Ödev 3:
 * LCD ekranın birinci satırında saat ve ADC değeri,
 * ikinci satırında ise sabit bir metin gösterilmiştir.
 */

// Saat değişkenleri
volatile uint8_t saat = 13;
volatile uint8_t dakika = 0;
volatile uint8_t saniye = 0;

volatile uint8_t guncelle = 1;
volatile uint32_t adcDeger = 0;

// Saat formatı: HH:MM:SS
void SaatString(char *buf)
{
    buf[0] = '0' + (saat / 10);
    buf[1] = '0' + (saat % 10);
    buf[2] = ':';
    buf[3] = '0' + (dakika / 10);
    buf[4] = '0' + (dakika % 10);
    buf[5] = ':';
    buf[6] = '0' + (saniye / 10);
    buf[7] = '0' + (saniye % 10);
    buf[8] = '\0';
}

// ADC değeri 4 haneli stringe çevrilir
void ADCString(uint32_t x, char *buf)
{
    buf[0] = '0' + (x / 1000);
    buf[1] = '0' + ((x % 1000) / 100);
    buf[2] = '0' + ((x % 100) / 10);
    buf[3] = '0' + (x % 10);
    buf[4] = '\0';
}

// Timer kesmesi (1 saniye)
void Timer0AIntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    saniye++;
    if (saniye == 60) { saniye = 0; dakika++; }
    if (dakika == 60) { dakika = 0; saat++; }
    if (saat == 24)   { saat = 0; }

    guncelle = 1;
}

// Timer ayarı
void TimerInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() - 1);

    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);
}

// ADC ayarı (PE3 - AIN0)
void ADCInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0,
                             ADC_CTL_CH0 | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
}

// ADC okuma
uint32_t ADCOku(void)
{
    uint32_t deger;
    ADCProcessorTrigger(ADC0_BASE, 3);
    while (!ADCIntStatus(ADC0_BASE, 3, false));
    ADCSequenceDataGet(ADC0_BASE, 3, &deger);
    return deger;
}

int main(void)
{
    char saatStr[9];
    char adcStr[5];
    char satir1[17];

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL |
                   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    baslangic();
    LCD_sil();

    ADCInit();
    TimerInit();
    IntMasterEnable();

    // İkinci satıra isim yazılır
    satir_sutun(2, 0);
    printf("MOONY");

    while (1)
    {
        if (guncelle)
        {
            guncelle = 0;

            SaatString(saatStr);
            adcDeger = ADCOku();
            ADCString(adcDeger, adcStr);

            // "HH:MM:SS␠␠XXXX"
            satir1[0] = saatStr[0];
            satir1[1] = saatStr[1];
            satir1[2] = saatStr[2];
            satir1[3] = saatStr[3];
            satir1[4] = saatStr[4];
            satir1[5] = saatStr[5];
            satir1[6] = saatStr[6];
            satir1[7] = saatStr[7];
            satir1[8] = ' ';
            satir1[9] = ' ';
            satir1[10] = adcStr[0];
            satir1[11] = adcStr[1];
            satir1[12] = adcStr[2];
            satir1[13] = adcStr[3];
            satir1[14] = ' ';
            satir1[15] = ' ';
            satir1[16] = '\0';

            satir_sutun(1, 0);
            printf(satir1);
        }
    }
}