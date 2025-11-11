https://github.com/user-attachments/assets/a47ddf0b-eb4f-456a-a784-0af1057aab03

Penjelasan Alur Kerja Task LED

1.  Deklarasi Pin LED #define LED_PIN 2 Pin 2 ditetapkan sebagai pin
    LED. Pendefinisian menggunakan #define membantu menjaga keteraturan
    kode dan memudahkan identifikasi pin yang digunakan.

2.  Deklarasi Task Handle TaskHandle_t hLED; hLED disediakan sebagai
    task handle yang digunakan untuk menyimpan referensi dari task LED.

3.  Pembuatan Task di setup() xTaskCreatePinnedToCore(taskLED,
    “taskLED”, 4000, NULL, 1, &hLED, 0); Fungsi ini akan membuat task
    LED ketika barisnya diaktifkan.

4.  Inisialisasi di dalam taskLED() pinMode(LED_PIN, OUTPUT); uint32_t
    t0 = millis(); Pin LED diatur sebagai output dan t0 digunakan
    menghitung durasi.

5.  Proses Utama taskLED() digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    vTaskDelay(200); LED ditoggle tiap 200 ms.

6.  Suspensi Setelah 5 Detik if (millis() - t0 > 5000) {
    vTaskSuspend(NULL); } Task berhenti sendiri setelah 5 detik.

