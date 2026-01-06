#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"

#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#include "lcd.h"

// ------------------------------------------------------------
// ÖDEV 2: LCD Saat Uygulaması
// - Timer0A kesmesi her 1 saniyede bir gelir
// - Kesmede sadece saat değişkenleri artırılır
// - LCD güncellemesi main döngüsünde yapılır (daha stabil)
// ------------------------------------------------------------

volatile uint8_t saat   = 0;
volatile uint8_t dakika = 0;
volatile uint8_t saniye = 0;

volatile uint8_t lcd_guncelle = 1;   // ilk açılışta ekrana yazdırmak için

// "SS:DD:SN" formatında string üretir (HH:MM:SS)
static void SaatStringOlustur(char *buf)
{
    buf[0] = '0' + (saat   / 10);
    buf[1] = '0' + (saat   % 10);
    buf[2] = ':';
    buf[3] = '0' + (dakika / 10);
    buf[4] = '0' + (dakika % 10);
    buf[5] = ':';
    buf[6] = '0' + (saniye / 10);
    buf[7] = '0' + (saniye % 10);
    buf[8] = '\0';
}

// Timer0A kesmesi: her 1 saniyede zamanı artırır
void Timer0AIntHandler(void)
{
    // Kesme bayrağını temizlemezsek sürekli kesmeye girer
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    saniye++;
    if (saniye >= 60) { saniye = 0; dakika++; }
    if (dakika >= 60) { dakika = 0; saat++;   }
    if (saat   >= 24) { saat = 0;             }

    // LCD'yi main içinde güncellemek için bayrak
    lcd_guncelle = 1;
}

static void Timer0A_Init_1s(void)
{
    // Timer0 periferini aç
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Periyodik mod
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    // 1 saniyelik periyot: load = clock
    // SysCtlClockGet() = sistem frekansı (örn. 50MHz)
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() - 1);

    // Kesme ayarları
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Timer'ı başlat
    TimerEnable(TIMER0_BASE, TIMER_A);
}

int main(void)
{
    char timeStr[9];

    // Sistem saati: 50 MHz (PLL aktif)
    // Saat doğruluğu büyük ölçüde bu clock ayarına bağlı
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL |
                   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // LCD başlat
    baslangic();
    LCD_sil();

    // Timer başlat
    Timer0A_Init_1s();

    // Global interrupt aç
    IntMasterEnable();

    // Ekran başlığı (istersen değiştir)
    satir_sutun(1, 0);
    printf("Dijital Saat");

    while (1)
    {
        if (lcd_guncelle)
        {
            lcd_guncelle = 0;

            SaatStringOlustur(timeStr);

            satir_sutun(2, 0);
            printf(timeStr);
            printf("        "); // eski karakter kalmasın diye boşluk
        }
    }
}