#include <Wire.h>
#include <MAX30105.h>
#include "heartRate.h"

MAX30105 sensor;

long lastBeat = 0;
int bpm = 0;
int beatCount = 0;

#define FINGER_THRESHOLD 8000   // Lower = faster detection

void setup() {
  Serial.begin(115200);
  delay(3000);

  Serial.println("=== MAX30102 HEART RATE ONLY ===");

  // ESP32 I2C pins
  Wire.begin(21, 22);
  delay(300);

  // FAST I2C (important for ESP32)
  if (!sensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 NOT detected");
    while (1);
  }

  Serial.println("MAX30102 detected");
  Serial.println("Place finger and wait...");

  // Optimized sensor setup for fast response
  sensor.setup(
    60,   // LED brightness (higher = faster lock)
    4,    // sample averaging (lower = faster)
    2,    // Red + IR
    100,  // sample rate
    411,  // pulse width
    4096  // ADC range
  );

  sensor.setPulseAmplitudeRed(0x1F);
  sensor.setPulseAmplitudeIR(0x1F);
  sensor.setPulseAmplitudeGreen(0);

  sensor.enableDIETEMPRDY();
}

void loop() {
  long irValue = sensor.getIR();

  // No finger detected
  if (irValue < FINGER_THRESHOLD) {
    beatCount = 0;
    lastBeat = 0;
    delay(100);
    return;
  }

  // Heartbeat detected
  if (checkForBeat(irValue)) {

    long now = millis();

    if (lastBeat > 0) {
      long delta = now - lastBeat;
      int rate = 60000 / delta;

      if (rate > 40 && rate < 150) {
        beatCount++;

        // Print after 2 stable beats
        if (beatCount >= 2) {
          bpm = rate;
          Serial.print("Heart Rate: ");
          Serial.print(bpm);
          Serial.println(" BPM");
        }
      }
    }

    lastBeat = now;
  }

  delay(10);  // Faster loop
}
