<img width="1915" height="912" alt="image" src="https://github.com/user-attachments/assets/b6174ce1-356e-4c61-9b36-06c3accd7825" />

https://github.com/user-attachments/assets/49cdab5f-2fb9-4252-9733-febbce1b64d6


------------------------------------------------------------
1. Definisi PIN
------------------------------------------------------------
#define POT_INPUT 5

Kode ini memberi nama POT_INPUT untuk pin GPIO 5 pada ESP32.
Pin ini akan dipakai untuk membaca nilai analog dari potensiometer.

------------------------------------------------------------
2. Handle Task
------------------------------------------------------------
TaskHandle_t hPot;

Variabel ini menyimpan "alamat" taskPot sehingga task lain (jika diperlukan)
dapat mengontrol, suspend, resume, atau delete taskPot. 
Meskipun di kode sekarang tidak digunakan untuk kontrol eksternal,
handle tetap disiapkan.

------------------------------------------------------------
3. Fungsi Task: taskPot
------------------------------------------------------------
void taskPot(void *pv) {
  int counter = 0;
  while (1) {
    int val = analogRead(POT_INPUT);
    Serial.printf("[POT] %d\n", val);

    counter++;
    if (counter == 10) {
      Serial.println("[POT] DELETE diri");
      vTaskDelete(NULL);
    }

    vTaskDelay(250);
  }
}

Penjelasan langkah demi langkah:

● int counter = 0;
  Sebuah penghitung internal untuk mengetahui berapa kali potensiometer
  sudah dibaca. Setiap loop = 1 kali pembacaan.

● while (1) { ... }
  Task berjalan terus tanpa henti, namun bisa berhenti jika task menghapus dirinya.

● int val = analogRead(POT_INPUT);
  ESP32 membaca tegangan dari pin POT_INPUT, lalu mengubahnya ke nilai digital
  0 sampai 4095 (karena ADC 12-bit).

● Serial.printf("[POT] %d\n", val);
  Nilai potensiometer ditampilkan pada Serial Monitor.

● counter++;
  Setiap selesai membaca, counter bertambah 1.

● if (counter == 10) vTaskDelete(NULL);
  Setelah 10 kali pembacaan, task menghapus dirinya sendiri.
  vTaskDelete(NULL) artinya: delete task yang sedang berjalan.

● vTaskDelay(250);
  Pause 250 ms (0.25 detik) sebelum loop berikutnya.
  Ini agar pembacaan tidak terlalu cepat.

------------------------------------------------------------
4. Pembuatan Task di setup()
------------------------------------------------------------
xTaskCreatePinnedToCore(taskPot, "taskPot", 3000, NULL, 1, &hPot, 0);

Penjelasan parameternya:

1. taskPot  
   Nama fungsi task yang akan dijalankan oleh FreeRTOS.

2. "taskPot"  
   Nama task, muncul di debugging FreeRTOS.

3. 3000  
   Ukuran stack untuk task (3000 byte sudah cukup untuk analogRead + Serial).

4. NULL  
   Parameter yang dikirim ke taskPot. Tidak digunakan.

5. 1  
   Prioritas task. Semakin tinggi angka, semakin penting.

6. &hPot  
   Alamat untuk menyimpan handle task. Disimpan di variabel hPot.

7. 0  
   Task dijalankan di core 0.

