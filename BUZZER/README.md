<img width="1915" height="907" alt="image" src="https://github.com/user-attachments/assets/e8c194a0-c168-49ca-ad9f-bab66c195490" />


https://github.com/user-attachments/assets/bd06893f-932a-4535-a11c-6d9e898b45f2


Buzzer Task Explanation

1.  Pendefinisian Pin #define BUZZER_PIN 46 Pin ini adalah GPIO tempat
    buzzer disambungkan.

2.  Struktur Task void taskBuzzer(void *pv) { pinMode(BUZZER_PIN,
    OUTPUT); while (1) { digitalWrite(BUZZER_PIN, HIGH);
    vTaskDelay(200); digitalWrite(BUZZER_PIN, LOW); vTaskDelay(200);
    Serial.printf(“[BUZZER] Core %d”, xPortGetCoreID()); } }

Task ini adalah thread FreeRTOS.

3.  pinMode pinMode(BUZZER_PIN, OUTPUT); GPIO 46 diatur sebagai output
    agar bisa mengontrol buzzer.

4.  Loop Utama Task digitalWrite(BUZZER_PIN, HIGH); vTaskDelay(200);
    digitalWrite(BUZZER_PIN, LOW); vTaskDelay(200);

Task menyalakan buzzer (HIGH) 200 ms, lalu mematikan 200 ms.
vTaskDelay() memberi waktu agar FreeRTOS melakukan multitasking.

5.  Debug Print Serial.printf(“[BUZZER] Core %d”, xPortGetCoreID());
    Menampilkan core mana yang menjalankan task ini.

