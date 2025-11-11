<img width="1917" height="911" alt="image" src="https://github.com/user-attachments/assets/e09efce7-8859-47b4-a2bb-02e66446915f" />

https://github.com/user-attachments/assets/95a68fdb-3caa-425c-8892-c02a6afe02e0

1. Definisi Pin
--------------------------------
#define SERVO_PIN 18

Bagian ini menentukan bahwa servo terhubung ke pin 18 pada ESP32. Dengan definisi ini,
penulisan di kode menjadi lebih jelas dan mudah dipahami.

2. Objek Servo
--------------------------------
Servo myservo;

Baris ini membuat sebuah objek servo bernama myservo yang akan digunakan untuk
mengontrol sudut servo melalui fungsi attach() dan write().

3. Inisialisasi Servo
--------------------------------
myservo.attach(SERVO_PIN);

Di dalam task, servo dihubungkan ke pin 18 menggunakan attach().
Tanpa langkah ini, servo tidak akan menerima sinyal PWM dari ESP32.

attach() mengaktifkan sinyal PWM sehingga servo bisa digerakkan.

4. Loop Pergerakan Servo
--------------------------------
while (1) {
    for (int p = 20; p <= 160; p += 4) {
      myservo.write(p);
      vTaskDelay(20);
    }
    for (int p = 160; p >= 20; p -= 4) {
      myservo.write(p);
      vTaskDelay(20);
    }

    Serial.println("[SERVO] Pause 1s");
    vTaskDelay(1000);
}

Task servo berjalan selamanya dalam loop tak berujung.

Bagian-bagiannya:

a. Gerakan maju (20 derajat ke 160 derajat)
   Servo bergerak dari posisi 20° naik ke 160° dengan langkah 4°.
   Setiap langkah diberi jeda 20 ms supaya servo bergerak halus.

b. Gerakan balik (160 derajat ke 20 derajat)
   Servo kembali turun dari 160° ke 20° dengan langkah -4°,
   tetap dengan delay 20 ms.

Servo jadi bergerak bolak-balik seperti wiper mobil.

c. Pause 1 detik
   Setelah satu siklus penuh (maju dan balik), task berhenti sejenak 1 detik.

5. vTaskDelay()
--------------------------------
vTaskDelay(20) membuat task servo memberi jeda kecil dalam tiap pergerakan,
sementara task lain tetap bisa berjalan.

vTaskDelay(1000) memberikan jeda 1 detik setelah satu siklus.

Berbeda dengan delay(), vTaskDelay() tidak menghentikan seluruh sistem,
melainkan hanya task ini saja.

6. Pembuatan Task
--------------------------------
xTaskCreatePinnedToCore(taskServo, "taskServo", 5000, NULL, 1, &hServo, 1);

Baris ini membuat task baru bernama "taskServo".

Parameter penting:
• taskServo   → fungsi task yang dijalankan
• 5000        → ukuran stack task
• prioritas 1 → task tidak terlalu penting namun tetap berjalan stabil
• hServo      → handle yang bisa dipakai untuk suspend/resume task
• core 1      → task dikunci agar berjalan di CPU core 1

Setelah task ini dibuat, servo akan bergerak bolak-balik secara otomatis.
