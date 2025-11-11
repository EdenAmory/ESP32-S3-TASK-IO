<img width="1918" height="914" alt="image" src="https://github.com/user-attachments/assets/27901990-8b3f-4b23-b916-2b343058d694" />


https://github.com/user-attachments/assets/f4c2e4f4-4b7b-4aa5-93c5-75786c5a8ba8


------------------------------------------------------------
1. DEFINISI PIN
------------------------------------------------------------

#define BUTTON_A       35
#define BUTTON_B       19

Dua pin tombol didefinisikan:
- BUTTON_A berada pada GPIO 35
- BUTTON_B berada pada GPIO 19

Tombol ini nanti digunakan oleh taskButton untuk membaca input.

------------------------------------------------------------
2. DEKLARASI HANDLE TASK
------------------------------------------------------------

TaskHandle_t hButton;

Variabel ini menyimpan "alamat" task button setelah dibuat. 
Handle ini berguna jika suatu saat ingin suspend, resume, atau delete task tersebut dari task lain.

------------------------------------------------------------
3. KODE TASK BUTTON
------------------------------------------------------------

void taskButton(void *pv) {
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);

  while (1) {
    int a = digitalRead(BUTTON_A);

    if (a == LOW) {
      Serial.println("[BUTTON] A ditekan -> Resume Buzzer");
      vTaskResume(hBuzzer);
    }

    vTaskDelay(100);
  }
}

Penjelasan baris per baris:

1. pinMode(BUTTON_A, INPUT_PULLUP);
   pinMode(BUTTON_B, INPUT_PULLUP);
   Kedua tombol menggunakan INPUT_PULLUP, artinya:
   - Kondisi idle = HIGH (karena ditarik ke 3.3V)
   - Ditekan = LOW (disambungkan ke GND)

2. while(1) { ... }
   Loop tak berakhir khas FreeRTOS task. Berjalan terus di core yang ditentukan.

3. int a = digitalRead(BUTTON_A);
   Membaca keadaan tombol A.

4. if (a == LOW) { ... }
   Artinya tombol A ditekan.

5. vTaskResume(hBuzzer);
   Jika tombol A ditekan, taskButton membangunkan (resume) task Buzzer, 
   karena taskBuzzer awalnya disuspend (vTaskSuspend(NULL) di dalam taskBuzzer).

6. vTaskDelay(100);
   Menunggu 100 ms agar:
   - Penggunaan CPU lebih efisien
   - Mencegah bouncing menyebabkan multi-trigger

------------------------------------------------------------
4. PEMBUATAN TASK DI SETUP()
------------------------------------------------------------

xTaskCreatePinnedToCore(
  taskButton,       // Fungsi task
  "taskButton",     // Nama task
  3000,             // Stack size dalam byte
  NULL,             // Parameter (tidak dipakai)
  2,                // Prioritas (lebih tinggi dari buzzer)
  &hButton,         // Mengisi handle task ke variabel hButton
  1                 // Task ditempatkan pada core 1
);

Penjelasan parameternya:

1. taskButton  
   Fungsi yang akan dijalankan sebagai task multitasking.

2. "taskButton"  
   Nama task untuk debugging.

3. 3000  
   Ukuran memory stack untuk task (3000 byte). Cukup untuk operasi sederhana.

4. NULL  
   Tidak ada data tambahan yang diberikan ke task.

5. Priority = 2  
   Artinya taskButton lebih penting daripada task yang punya priority 1.

6. &hButton  
   FreeRTOS akan menyimpan handle task yang baru dibuat ke variabel ini.
