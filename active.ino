#define BUZZER_PIN 26   // Safe GPIO on ESP32

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  digitalWrite(BUZZER_PIN, HIGH); // ON
  delay(2000);

  digitalWrite(BUZZER_PIN, LOW);  // OFF
  delay(2000);
}
