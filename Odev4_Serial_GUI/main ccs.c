#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"

/* =========================================================
 * ÖDEV 4 - UART (PC GUI) + LCD + ADC + Buton
 *
 * - PC'den komut alınır:
 *   'S' + "HH:MM:SS"  -> saati ayarlar
 *   'M' + "ABC"       -> LCD'de gösterilecek 3 karakterlik mesajı ayarlar
 *
 * - Her 1 saniyede bir:
 *   ADC okunur, buton durumu kontrol edilir,
 *   PC'ye rapor gönderilir ve LCD güncellenir.
 * ========================================================= */

// --- Donanım Eşlemesi (LCD) ---
#define LCD_PORT_BASE GPIO_PORTB_BASE
#define RS GPIO_PIN_0
#define E  GPIO_PIN_1
#define D4 GPIO_PIN_4
#define D5 GPIO_PIN_5
#define D6 GPIO_PIN_6
#define D7 GPIO_PIN_7

// --- GLOBAL DEĞİŞKENLER ---
volatile int hours = 0, minutes = 0, seconds = 0;

volatile uint32_t adcValue[1];

// LCD'nin ikinci satırında gösterilecek kısa mesaj (PC'den değiştirilebilir)
char lcd_custom_msg[8] = "---";

// 1 saniyelik tetik için bayrak
volatile bool send_report_flag = false;

// Buton için "yakalama" mantığı: 1 saniye içinde bir kere bile basılırsa 1 raporlanır
bool button_latch = false;

// Buffer'lar
char l1[64];
char l2[64];
char txBuf[64];

/* =========================
 * LCD Sürücü (4-bit)
 * ========================= */

void LCD_Pulse_Enable() {
    // Enable pini kısa süre 1 yapılıp tekrar 0 yapılır (LCD veri okur)
    GPIOPinWrite(LCD_PORT_BASE, E, E);
    SysCtlDelay(40000);
    GPIOPinWrite(LCD_PORT_BASE, E, 0);
    SysCtlDelay(40000);
}

void LCD_Write_4Bit(unsigned char data) {
    // D4..D7 hatlarına 4 bit gönderilir (üst nibble)
    GPIOPinWrite(LCD_PORT_BASE, D4|D5|D6|D7, (data << 4));
    LCD_Pulse_Enable();
}

void LCD_Cmd(unsigned char cmd) {
    // RS=0 -> komut modu
    GPIOPinWrite(LCD_PORT_BASE, RS, 0);
    LCD_Write_4Bit(cmd >> 4);
    LCD_Write_4Bit(cmd & 0x0F);
    SysCtlDelay(80000);
}

void LCD_Data(unsigned char data) {
    // RS=1 -> veri modu
    GPIOPinWrite(LCD_PORT_BASE, RS, RS);
    LCD_Write_4Bit(data >> 4);
    LCD_Write_4Bit(data & 0x0F);
    SysCtlDelay(80000);
}

void LCD_Init() {
    // LCD pinleri PB portunda çıkış yapılır
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(LCD_PORT_BASE, 0xFF);
    SysCtlDelay(1000000);

    // 4-bit moda geçiş için standart başlangıç dizisi
    LCD_Write_4Bit(0x03); SysCtlDelay(100000);
    LCD_Write_4Bit(0x03); SysCtlDelay(100000);
    LCD_Write_4Bit(0x03); SysCtlDelay(100000);
    LCD_Write_4Bit(0x02);

    // 2 satır, 5x8 font, ekran açık, giriş modu, ekran temizle
    LCD_Cmd(0x28);
    LCD_Cmd(0x0C);
    LCD_Cmd(0x06);
    LCD_Cmd(0x01);
    SysCtlDelay(200000);
}

void LCD_Print(char *str) {
    while(*str) LCD_Data(*str++);
}

/* =========================
 * TIMER KESMESİ (1 saniye)
 * ========================= */

void Timer0IntHandler(void) {
    // 1 saniye geçti -> saat artır ve ana döngüye "güncelle" de
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    if(++seconds >= 60) {
        seconds = 0;
        if(++minutes >= 60) {
            minutes = 0;
            if(++hours >= 24) hours = 0;
        }
    }
    send_report_flag = true;
}

/* =========================
 * DONANIM BAŞLATMA
 * ========================= */

void InitHardware() {
    // Sistem saati ayarı (PLL ile)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    /* -------- UART0 (PC haberleşmesi) -------- */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    /* -------- Timer0 (1 saniye) -------- */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    // Load = SysClk -> yaklaşık 1 saniye
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());

    // Timer kesmesini bağla ve aktif et
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    /* -------- ADC0 (PE3 / AIN0) -------- */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // PE3 analog giriş
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    // Sequencer 3: tek örnekleme
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);

    /* -------- Buton (PF4) -------- */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // PortF için kilit açma (özellikle PF0 için bilinen durum, burada alışkanlık)
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    // PF4 giriş + pull-up (buton basılınca 0 okunur)
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Global interrupt aç + timer başlat
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);
}

int main(void) {
    InitHardware();
    LCD_Init();

    adcValue[0] = 0;

    while (1) {

        /* -------------------------------
         * 0) Buton "yakalama" (polling)
         * Bu kısım sürekli döndüğü için kısa basma da kaçmaz.
         * ------------------------------- */
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0) {
            button_latch = true;
        }

        /* -------------------------------
         * 1) PC'den komut alma
         * 'S' : saat ayarı
         * 'M' : 3 karakter mesaj ayarı
         * ------------------------------- */
        if (UARTCharsAvail(UART0_BASE)) {
            char cmd = UARTCharGet(UART0_BASE);

            if (cmd == 'S') {
                // Format: HH:MM:SS (8 karakter)
                char b[9]; int i;
                for(i=0; i<8; i++) {
                    while(!UARTCharsAvail(UART0_BASE));
                    b[i] = UARTCharGet(UART0_BASE);
                }
                b[8] = '\0';

                // "HH:MM:SS" -> saat/dakika/saniye ayrıştırma
                hours   = atoi(b);
                minutes = atoi(b + 3);
                seconds = atoi(b + 6);
            }
            else if (cmd == 'M') {
                // 3 karakterlik kısa mesaj
                int i;
                for(i=0; i<3; i++) {
                    while(!UARTCharsAvail(UART0_BASE));
                    lcd_custom_msg[i] = UARTCharGet(UART0_BASE);
                }
                lcd_custom_msg[3] = '\0';
            }
        }

        /* -------------------------------
         * 2) 1 saniyede bir rapor + LCD güncelle
         * Timer kesmesi send_report_flag'i 1 yapar.
         * ------------------------------- */
        if (send_report_flag) {
            send_report_flag = false;

            /* 2.1 ADC oku */
            ADCProcessorTrigger(ADC0_BASE, 3);
            while(!ADCIntStatus(ADC0_BASE, 3, false));
            ADCIntClear(ADC0_BASE, 3);
            ADCSequenceDataGet(ADC0_BASE, 3, (uint32_t*)adcValue);

            /* 2.2 Buton durumu: 1 saniyede basıldı mı? */
            int btn = button_latch ? 1 : 0;
            button_latch = false;

            /* 2.3 PC'ye rapor gönder (GUI burada okuyabilir)
             * Format: HH:MM:SS;ADC;BTN
             */
            sprintf(txBuf, "%02d:%02d:%02d;%u;%d\r\n",
                    hours, minutes, seconds, adcValue[0], btn);

            char *p = txBuf;
            while(*p) UARTCharPut(UART0_BASE, *p++);

            /* 2.4 LCD güncelle */
            sprintf(l1, "Time: %02d:%02d:%02d", hours, minutes, seconds);
            LCD_Cmd(0x80);
            LCD_Print(l1);

            sprintf(l2, "ADC:%4u Msg:%s", adcValue[0], lcd_custom_msg);
            LCD_Cmd(0xC0);
            LCD_Print(l2);
        }
    }
}