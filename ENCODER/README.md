<img width="1915" height="919" alt="image" src="https://github.com/user-attachments/assets/a111eae6-301e-4280-9f2e-8a34aac95f22" />

https://github.com/user-attachments/assets/908107e8-aaa5-4630-92d7-f1d9a4ba57cd


1. Definisi Pin
--------------------------------
#define ENC_CLK 40
#define ENC_DT  41

Dua baris ini mendefinisikan bahwa rotary encoder menggunakan:
- ENC_CLK pada pin 40
- ENC_DT pada pin 41

2. Inisialisasi Pin Encoder
--------------------------------
pinMode(ENC_CLK, INPUT_PULLUP);
pinMode(ENC_DT, INPUT_PULLUP);

Kedua pin diset sebagai INPUT_PULLUP, artinya:
- ESP32 memberi resistor pull-up internal
- HIGH ketika idle, berubah ketika encoder diputar

3. Variabel Pembacaan Awal
--------------------------------
int lastClk = digitalRead(ENC_CLK);
int counter = 0;

lastClk menyimpan kondisi sebelumnya untuk mendeteksi tepi.
counter menghitung jumlah pulse untuk jeda setiap 20 langkah.

4. Loop Task Encoder
--------------------------------
while (1) {
    int c = digitalRead(ENC_CLK);

    if (lastClk == HIGH && c == LOW) {
        if (digitalRead(ENC_DT) == HIGH) encValue++;
        else encValue--;

        Serial.printf("[ENC] %d\n", encValue);

        counter++;
        if (counter == 20) {
            Serial.println("[ENC] PAUSE 100ms");
            vTaskDelay(100);
            counter = 0;
        }
    }

    lastClk = c;
    vTaskDelay(1);
}

Saat terjadi transisi HIGH→LOW pada pin CLK, task membaca pin DT:
- DT HIGH berarti putar kanan (encValue++)
- DT LOW berarti putar kiri (encValue--)

5. Pembuatan Task
--------------------------------
xTaskCreatePinnedToCore(taskEncoder, "taskEncoder", 4000, NULL, 1, &hEncoder, 0);

Baris ini membuat task encoder pada core 0 dengan prioritas 1 dan stack 4000 byte.
