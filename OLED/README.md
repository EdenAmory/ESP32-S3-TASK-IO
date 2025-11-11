<img width="1920" height="922" alt="image" src="https://github.com/user-attachments/assets/f85e810e-90dc-4a20-9a4b-6fc44b436b79" />

https://github.com/user-attachments/assets/a7a967af-e123-4f95-9282-a8b4691d8707


1. Definisi Pin OLED
   Pada bagian awal kode, terdapat dua define:
   - OLED_SDA 38
   - OLED_SCL 39
   Dua pin ini digunakan oleh protokol I2C untuk berkomunikasi dengan layar OLED.
   SDA = jalur data. 
   SCL = jalur clock sinkronisasi.
   OLED memakai I2C sehingga hanya dua pin ini yang dibutuhkan untuk transfer data.

2. Objek OLED
   Adafruit_SSD1306 screen(128, 64, &Wire, -1);
   Baris ini membuat objek layar bernama screen. 
   - 128 x 64 adalah resolusi pixel.
   - &Wire berarti memakai bus I2C default, tapi pin SDA/SCL nanti ditentukan di Wire.begin.
   - -1 berarti OLED ini tidak memakai pin reset eksternal.

3. Masuk ke taskOLED()
   void taskOLED(void *pv) {

   Bagian ini adalah fungsi task FreeRTOS yang akan dijalankan pada core tertentu.

4. Inisialisasi I2C dan Layar
   Wire.begin(OLED_SDA, OLED_SCL);
   - Fungsi ini memulai komunikasi I2C menggunakan pin yang sudah ditentukan.

   screen.begin(SSD1306_SWITCHCAPVCC, 0x3C);
   - Memulai OLED dengan alamat I2C 0x3C.
   - SSD1306_SWITCHCAPVCC artinya tegangan Vcc diatur secara internal oleh library SSD1306.

5. Variabel penghitung
   int c = 0;
   - Variabel ini dipakai untuk menghitung berapa kali layar telah di-update.

6. Perulangan Utama Task
   while (1) {

   OLED akan terus bekerja selamanya selama task ini tidak dihapus atau sistem mati.

7. Membersihkan layar
   screen.clearDisplay();
   - Menghapus tampilan layar agar frame berikutnya bersih.

8. Menulis teks baris pertama
   screen.setCursor(0, 0);
   screen.println("OLED OK");
   - Menulis teks “OLED OK” pada posisi (0,0).

9. Menulis teks baris kedua
   screen.setCursor(0, 20);
   screen.printf("Count %d", c);
   - Menampilkan nilai counter di layar.

10. Menampilkan ke layar
    screen.display();
    - Perintah ini sangat penting karena tanpa ini OLED tidak akan memperbarui tampilan.

11. Increment counter
    c++;

12. Mekanisme Delay Khusus
    if (c % 3 == 0) {
        Serial.println("[OLED] PAUSE 1s");
        vTaskDelay(1000);
    }
    - Setiap 3 kali update, taskOLED berhenti selama 1 detik.
    - Ini contoh pemakaian vTaskDelay untuk jeda dalam task.

13. Delay rutin
    vTaskDelay(300);
    - Delay 300 ms agar OLED tidak diperbarui terlalu cepat.

14. Task selesai dijalankan oleh core tertentu
    Saat setup(), OLED dibuat dengan:
    xTaskCreatePinnedToCore(taskOLED, "taskOLED", 5000, NULL, 1, &hOLED, 1);

    Arti parameternya:
    - taskOLED = fungsi yang dijalankan sebagai task
    - "taskOLED" = nama task
    - 5000 = ukuran stack (byte)
    - NULL = tidak ada parameter yang diberikan ke task
    - 1 = prioritas task
    - &hOLED = FreeRTOS menyimpan handle task di variabel ini
    - 1 = task berjalan pada core 1 ESP32


