<img width="1919" height="909" alt="image" src="https://github.com/user-attachments/assets/83a682bf-3653-43a5-9635-27f704ef8ec7" />

https://github.com/user-attachments/assets/061044ac-f9f8-4c8f-b76d-ebf315a6a0bd


1.  Definisi Pin Stepper Empat pin stepper didefinisikan:

    -   STP1 = 4
    -   STP2 = 7
    -   STP3 = 8
    -   STP4 = 9

    Empat pin ini digunakan oleh AccelStepper untuk mengendalikan
    stepper dalam mode FULL4WIRE.

2.  Inisialisasi Objek Stepper Baris ini membuat objek motor:
    motor(AccelStepper::FULL4WIRE, STP1, STP3, STP2, STP4);

    Artinya:

    -   Stepper memakai mode FULL4WIRE.
    -   Urutan pin ke library: IN1 = STP1, IN2 = STP3, IN3 = STP2, IN4 =
        STP4.

3.  Task Stepper Dibuat di setup() xTaskCreatePinnedToCore(taskStepper,
    “taskStepper”, 6000, NULL, 1, &hStepper, 1);

    Ini menjalankan taskStepper di core 1. Prioritas = 1.

4.  Masuk ke Fungsi taskStepper() Setelah task berjalan, eksekusi masuk
    ke loop taskStepper.

5.  Set Parameter Motor motor.setMaxSpeed(400);
    motor.setAcceleration(150);

    -   Maksimal kecepatan = 400 step/detik
    -   Akselerasi = 150 step/detik^2

6.  Pergerakan Stepper Bagian Pertama motor.moveTo(300);

    Ini memberi perintah: stepper harus bergerak menuju posisi 300.

    Lalu: while (motor.distanceToGo()) motor.run();

    motor.run() wajib dipanggil berulang. Loop ini akan terus berputar
    sampai posisi tercapai.

7.  Pergerakan Stepper Bagian Kedua (Balik Arah) motor.moveTo(-200);

    Sama seperti sebelumnya, tapi tujuannya ke posisi negatif.

    while (motor.distanceToGo()) motor.run();

8.  Setelah Bolak Balik Selesai Setelah mencapai -200, pergerakan satu
    cycle selesai.

    Serial.println(“[STEPPER] Suspend 2s”);

    Lalu: vTaskDelay(2000);

    Task “tidur” selama 2 detik.

9.  Perulangan Tak Hingga Karena taskStepper berada dalam while(1),
    pergerakan maju 300, mundur ke -200, lalu delay 2 detik akan terus
    berulang tanpa henti.
