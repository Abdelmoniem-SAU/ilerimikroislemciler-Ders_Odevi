# Ödev 4 - Seri Port ve GUI Haberleşmesi (UART)

## Amaç
Bu ödevde Tiva TM4C123 ile PC arasında UART (seri haberleşme) kuruldu.
PC tarafında C# (SharpDevelop) ile hazırlanan arayüz üzerinden:
- Tiva’ya saat bilgisi gönderiliyor,
- LCD’de gösterilecek 3 karakterlik mesaj gönderiliyor,
- Tiva’dan her saniye gelen saat/ADC/buton bilgisi ekranda gösteriliyor.

## Donanım / Bağlantı
- UART0 kullanıldı:
  - PA0 = U0RX
  - PA1 = U0TX
- Baudrate: **9600**, 8N1

Not: USB ile bağlandığında Windows’ta görünen COM port numarası (ör: COM4) GUI’de yazılır.

## GUI (SharpDevelop) Nasıl Çalışır?
Arayüzde 3 ana işlem var:

### 1) Start/Stop (Bağlantı)
- `txtPort` içine COM port yazılır (ör: **COM4**).
- **Start** butonuna basınca port açılır.
- Tekrar basınca port kapanır (Stop/Start şeklinde).

### 2) Saat Gönderme (Sync Time)
- `txtTimeIn` alanına saat **HH:MM:SS** formatında yazılır (ör: 13:25:00).
- **Sync Time** butonuna basınca PC -> Tiva şu format gönderir:
  - `S` + `HH:MM:SS`
  - Örnek: `S13:25:00`

### 3) Mesaj Gönderme (3 karakter)
- `txtMsgIn` alanına LCD’de görünecek 3 karakter yazılır (ör: `ABC`).
- **Update Display** butonuna basınca PC -> Tiva şu format gönderir:
  - `M` + `XXX` (tam 3 karakter)
  - 3’ten kısa ise boşlukla tamamlanır, uzunsa kesilir.
  - Örnek: `MMNY`

## Tiva -> PC Veri Akışı
Tiva her **1 saniyede** bir PC’ye rapor yollar.
Format:

`HH:MM:SS;ADC;BTN`

- `HH:MM:SS` : Güncel saat
- `ADC`       : ADC ham değeri (0..4095 gibi)
- `BTN`       : 1 = son 1 saniye içinde butona basıldı, 0 = basılmadı

Örnek satır:
`13:00:12;4095;1`

GUI tarafında `DataReceived` olayı ile bu satır okunur, `;` ile parçalanır ve
- `txtTimeOut` -> saat,
- `txtAdcOut`  -> ADC,
- `txtStatus`  -> Pressed/Released
alanlarına yazdırılır.

## Çalıştırma Adımları
1. Tiva kodunu karta yükle.
2. Kartı USB ile PC’ye bağla, COM port numarasını öğren (Aygıt Yöneticisi).
3. GUI’yi çalıştır.
4. `txtPort` = COMx yaz.
5. Start ile bağlantıyı aç.
6. `txtTimeIn` gir -> Sync Time.
7. `txtMsgIn` gir -> Update Display.
8. Her saniye gelen veriler GUI’de güncellenecek.
