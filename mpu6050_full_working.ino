#include <Wire.h>

#define MPU_ADDR 0x68
#define ACCEL_XOUT_H 0x3B
#define PWR_MGMT_1   0x6B

void setup() {
  Serial.begin(115200);
  delay(2000);

  Wire.begin(21, 22);
  Wire.setClock(100000);

  // Wake MPU
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);
  Wire.endTransmission();

  Serial.println("MPU clone initialized (±2g mode)");
}

void loop() {
  int16_t ax, ay, az;

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6);

  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();

  // Correct scaling for ±2g
  float axg = ax / 16384.0;
  float ayg = ay / 16384.0;
  float azg = az / 16384.0;

  Serial.print("X: "); Serial.print(axg, 2);
  Serial.print(" g | Y: "); Serial.print(ayg, 2);
  Serial.print(" g | Z: "); Serial.print(azg, 2);
  Serial.println(" g");

  delay(1000);
}
