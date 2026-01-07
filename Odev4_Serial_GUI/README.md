Ödev 4 - Seri Port ve GUI Haberleşmesi

Bu ödevde Tiva TM4C123 ile bilgisayar arasında UART üzerinden haberleşme yapılmıştır.
Bilgisayar tarafında SharpDevelop kullanılarak basit bir GUI hazırlanmıştır.

GUI üzerinden:
- Tiva kartına saat bilgisi gönderilmektedir.
- LCD’de gösterilecek 3 karakterlik mesaj gönderilmektedir.
- Tiva’dan her saniye gelen saat, ADC ve buton bilgileri ekranda gösterilmektedir.

Bağlantı:
UART0 kullanılmıştır (PA0 RX, PA1 TX).
Baudrate 9600 olarak ayarlanmıştır.
GUI’de COM port numarası (örnek: COM4) girilerek bağlantı açılır.

Butonlar:
- Start/Stop: Seri port bağlantısını açar ve kapatır.
- Sync Time: Girilen saati (HH:MM:SS) Tiva’ya gönderir.
- Update Display: LCD’de gösterilecek 3 karakterlik mesajı gönderir.

Veri Akışı:
Tiva kartı her 1 saniyede bir PC’ye veri gönderir.
Gönderilen veri formatı:

HH:MM:SS;ADC;BTN

Burada:
- HH:MM:SS anlık saati,
- ADC okunan analog değeri,
- BTN ise butona basılıp basılmadığını göstermektedir.

GUI tarafında gelen veri okunur ve ilgili alanlara yazdırılır.
