#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>
#include <AccelStepper.h>

#define LED_PIN        2
#define BUZZER_PIN     46
#define BUTTON_A       35
#define BUTTON_B       19
#define POT_INPUT      5
#define ENC_CLK        40
#define ENC_DT         41
#define OLED_SDA       38
#define OLED_SCL       39
#define SERVO_PIN      18

#define STP1 4
#define STP2 7
#define STP3 8
#define STP4 9

Adafruit_SSD1306 screen(128, 64, &Wire, -1);
Servo myservo;
AccelStepper motor(AccelStepper::FULL4WIRE, STP1, STP3, STP2, STP4);

volatile int encValue = 0;

// -------- TASK HANDLES --------
TaskHandle_t hLED;
TaskHandle_t hBuzzer;
TaskHandle_t hButton;
TaskHandle_t hPot;
TaskHandle_t hOLED;
TaskHandle_t hEncoder;
TaskHandle_t hServo;
TaskHandle_t hStepper;


// ======================= LED TASK ==========================
//LED jalan 5 detik, lalu suspend dirinya
void taskLED(void *pv) {
  pinMode(LED_PIN, OUTPUT);
  uint32_t t0 = millis();

  while (1) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    Serial.printf("[LED]\n");

    if (millis() - t0 > 5000) {
      Serial.println("[LED] SUSPEND DIRI");
      vTaskSuspend(NULL);
    }
    vTaskDelay(200);
  }
}


// ======================= BUZZER TASK ==========================
//Buzzer idle sampai di-resume oleh taskButton
void taskBuzzer(void *pv) {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.println("[BUZZER] Menunggu resume...");
  vTaskSuspend(NULL);   // mulai dalam keadaan pause

  while (1) {
    digitalWrite(BUZZER_PIN, HIGH);
    vTaskDelay(150);
    digitalWrite(BUZZER_PIN, LOW);
    vTaskDelay(150);
    Serial.println("[BUZZER]");
  }
}


// ======================= BUTTON TASK ==========================
//menekan tombol A = resume buzzer
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


// ======================= POT TASK ==========================
//Setelah membaca 10 kali, task dihapus
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


// ======================= OLED TASK ==========================
//OLED berhenti 1 detik tiap 3 kali update
void taskOLED(void *pv) {
  Wire.begin(OLED_SDA, OLED_SCL);
  screen.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  int c = 0;
  while (1) {
    screen.clearDisplay();
    screen.setCursor(0, 0);
    screen.println("OLED OK");
    screen.setCursor(0, 20);
    screen.printf("Count %d", c);
    screen.display();

    c++;

    if (c % 3 == 0) {
      Serial.println("[OLED] PAUSE 1s");
      vTaskDelay(1000);
    }

    vTaskDelay(300);
  }
}


// ======================= ENCODER TASK ==========================
// pause 100ms tiap 20 pulse
void taskEncoder(void *pv) {
  pinMode(ENC_CLK, INPUT_PULLUP);
  pinMode(ENC_DT, INPUT_PULLUP);

  int lastClk = digitalRead(ENC_CLK);
  int counter = 0;

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
}



// ======================= SERVO TASK ==========================
//setiap satu cycle, servo pause 1 detik
void taskServo(void *pv) {
  myservo.attach(SERVO_PIN);

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
}


// ======================= STEPPER TASK ==========================
//stepper suspend 2 detik tiap selesai bolak-balik
void taskStepper(void *pv) {
  motor.setMaxSpeed(400);
  motor.setAcceleration(150);

  while (1) {
    motor.moveTo(300);
    while (motor.distanceToGo()) motor.run();

    motor.moveTo(-200);
    while (motor.distanceToGo()) motor.run();

    Serial.println("[STEPPER] Suspend 2s");
    vTaskDelay(2000);
  }
}



// ======================= SETUP ==========================
void setup() {
  Serial.begin(115200);

  // LED
  //xTaskCreatePinnedToCore(taskLED, "taskLED", 4000, NULL, 1, &hLED, 0);

  // BUZZER
  //xTaskCreatePinnedToCore(taskBuzzer, "taskBuzzer", 3000, NULL, 1, &hBuzzer, 0);

  // BUTTON
 // xTaskCreatePinnedToCore(taskButton, "taskButton", 3000, NULL, 1, &hButton, 1);

  // POT
 // xTaskCreatePinnedToCore(taskPot, "taskPot", 3000, NULL, 1, &hPot, 0);

  // OLED
 // xTaskCreatePinnedToCore(taskOLED, "taskOLED", 5000, NULL, 1, &hOLED, 1);

  // ENC
  xTaskCreatePinnedToCore(taskEncoder, "taskEncoder", 4000, NULL, 1, &hEncoder, 0);

  // SERVO
  //xTaskCreatePinnedToCore(taskServo, "taskServo", 5000, NULL, 1, &hServo, 1);

  // STEPPER
 // xTaskCreatePinnedToCore(taskStepper, "taskStepper", 6000, NULL, 1, &hStepper, 1);
}

void loop() {}
