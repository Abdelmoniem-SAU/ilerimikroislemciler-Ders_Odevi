Ödev 2 - LCD Saat Uygulaması

Bu ödevde Tiva TM4C123 kullanılarak LCD ekran üzerinde dijital saat gösterilmiştir.
Saat bilgisi HH:MM:SS formatında LCD’ye yazdırılmaktadır.

Saatin çalışması Timer0 kesmesi ile sağlanmıştır.
Timer her 1 saniyede bir kesme oluşturarak saniye değerini artırmaktadır.
Saniye 60 olduğunda dakika, dakika 60 olduğunda saat artırılmaktadır.

LCD güncellemesi ana döngü içinde yapılmıştır.
Bu şekilde kesme içinde LCD yazma işlemi yapılmamış ve sistem daha stabil çalışmıştır.

Saat doğruluğu, sistem saat ayarına (PLL ve XTAL) bağlıdır.
