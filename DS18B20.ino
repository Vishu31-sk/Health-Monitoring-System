#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4   // GPIO where DS18B20 is connected

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("=== DS18B20 Temperature Test ===");
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures();  // Send command to get temperature

  float tempC = sensors.getTempCByIndex(0);

  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("❌ DS18B20 not detected");
  } else {
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println(" °C");
  }

  delay(2000);
}
