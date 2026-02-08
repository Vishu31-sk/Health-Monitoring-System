/* ================= BLYNK DETAILS ================= */
#define BLYNK_TEMPLATE_ID "TMPL3sk51JqqB"
#define BLYNK_TEMPLATE_NAME "worker safety"
#define BLYNK_AUTH_TOKEN "-LvY4IM-3O-sWDMCWUjAd6BrKC7eNIq2"

#include <Wire.h>
#include <DHT.h>
#include <MAX30105.h>
#include <spo2_algorithm.h>

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Krishna 502 4G";
char pass[] = "Tanish@123";

/* ================= PIN DEFINITIONS ================= */
#define MQ135_PIN     34
#define DHTPIN        4
#define BUZZER_PIN    26
#define DHTTYPE       DHT11

/* ================= THRESHOLDS ================= */
#define MQ135_THRESHOLD   2000
#define TEMP_THRESHOLD    40.0
#define HUM_THRESHOLD     90.0
#define MOTION_THRESHOLD  2.5
#define BPM_LOW           45
#define BPM_HIGH          130

/* ================= MPU6050 ================= */
#define MPU_ADDR        0x68
#define ACCEL_XOUT_H    0x3B
#define PWR_MGMT_1      0x6B

/* ================= OBJECTS ================= */
DHT dht(DHTPIN, DHTTYPE);
MAX30105 particleSensor;

/* ================= MAX30102 BUFFERS ================= */
#define BUFFER_SIZE 50
#define SHIFT_SIZE  10

uint32_t irBuffer[BUFFER_SIZE];
uint32_t redBuffer[BUFFER_SIZE];

int32_t spo2;
int8_t validSPO2;
int32_t heartRate;
int8_t validHeartRate;

/* ================= FILTERED OUTPUT ================= */
int bpmFiltered = 0;
int spo2Filtered = 0;

/* ================= MPU VALUES ================= */
float axg = 0, ayg = 0, azg = 0;

/* ================= BUZZER ================= */
bool buzzerOn = false;
unsigned long buzzerStart = 0;
#define BUZZ_TIME 5   // 5 ms

void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(MQ135_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Wire.begin(21, 22);
  Wire.setClock(400000);

  dht.begin();

  /* ---------- WiFi + Blynk ---------- */
  WiFi.begin(ssid, pass);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  /* ---------- MPU6050 INIT ---------- */
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);
  Wire.endTransmission();

  Serial.println("✅ MPU6050 Initialized");

  /* ---------- MAX30102 ---------- */
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("❌ MAX30102 NOT detected");
    while (1);
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);
  particleSensor.setPulseAmplitudeGreen(0);

  Serial.println("✅ MAX30102 Ready");

  /* ---------- INITIAL BUFFER ---------- */
  for (int i = 0; i < BUFFER_SIZE; i++) {
    while (!particleSensor.available())
      particleSensor.check();

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i]  = particleSensor.getIR();
    particleSensor.nextSample();
  }
}

/* ================= LOOP ================= */
void loop() {

  Blynk.run();

  /* ---------- MQ135 ---------- */
  int air = analogRead(MQ135_PIN);

  /* ---------- DHT ---------- */
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  /* ---------- MPU6050 READ ---------- */
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);

  if (Wire.requestFrom(MPU_ADDR, 6) == 6) {
    int16_t ax = Wire.read() << 8 | Wire.read();
    int16_t ay = Wire.read() << 8 | Wire.read();
    int16_t az = Wire.read() << 8 | Wire.read();

    axg = ax / 16384.0;
    ayg = ay / 16384.0;
    azg = az / 16384.0;
  }

  float motion = abs(axg) + abs(ayg) + abs(azg);

  /* ---------- MAX30102 BUFFER UPDATE ---------- */
  for (int i = SHIFT_SIZE; i < BUFFER_SIZE; i++) {
    redBuffer[i - SHIFT_SIZE] = redBuffer[i];
    irBuffer[i - SHIFT_SIZE]  = irBuffer[i];
  }

  for (int i = BUFFER_SIZE - SHIFT_SIZE; i < BUFFER_SIZE; i++) {
    while (!particleSensor.available())
      particleSensor.check();

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i]  = particleSensor.getIR();
    particleSensor.nextSample();
  }

  maxim_heart_rate_and_oxygen_saturation(
    irBuffer, BUFFER_SIZE,
    redBuffer,
    &spo2, &validSPO2,
    &heartRate, &validHeartRate
  );

  if (validHeartRate && heartRate >= BPM_LOW && heartRate <= BPM_HIGH) {
    if (bpmFiltered == 0) bpmFiltered = heartRate;
    bpmFiltered = (bpmFiltered * 4 + heartRate) / 5;
  }

  if (validSPO2 && spo2 >= 92 && spo2 <= 99) {
    if (spo2Filtered == 0) spo2Filtered = spo2;
    spo2Filtered = (spo2Filtered * 3 + spo2) / 4;
  }

  /* ---------- TRIGGER ---------- */
  bool trigger =
    air > MQ135_THRESHOLD ||
    temp > TEMP_THRESHOLD ||
    hum > HUM_THRESHOLD ||
    motion > MOTION_THRESHOLD ||
    (bpmFiltered > 0 && (bpmFiltered < BPM_LOW || bpmFiltered > BPM_HIGH));

  if (trigger && !buzzerOn) {
    buzzerOn = true;
    buzzerStart = millis();
    digitalWrite(BUZZER_PIN, HIGH);
  }

  if (buzzerOn && millis() - buzzerStart >= BUZZ_TIME) {
    buzzerOn = false;
    digitalWrite(BUZZER_PIN, LOW);
  }

  /* ---------- BLYNK UPDATE ---------- */
  Blynk.virtualWrite(V0, air);          // MQ135
  Blynk.virtualWrite(V2, hum);          // Humidity
  Blynk.virtualWrite(V1, temp);         // Temperature
  Blynk.virtualWrite(V3, bpmFiltered);  // BPM
  Blynk.virtualWrite(V4, spo2Filtered); // SpO2
  Blynk.virtualWrite(V5, axg);           // MPU X
  Blynk.virtualWrite(V6, ayg);           // MPU Y
  Blynk.virtualWrite(V7, azg);           // MPU Z

  /* ---------- SERIAL OUTPUT ---------- */
  Serial.print("MQ135: "); Serial.print(air);
  Serial.print(" | Temp: "); Serial.print(temp);
  Serial.print(" | Hum: "); Serial.print(hum);

  Serial.print(" | AX: "); Serial.print(axg, 2);
  Serial.print(" AY: "); Serial.print(ayg, 2);
  Serial.print(" AZ: "); Serial.print(azg, 2);

  Serial.print(" | BPM: "); Serial.print(bpmFiltered);
  Serial.print(" | SpO2: "); Serial.print(spo2Filtered);
  Serial.print(" | TRIGGER: ");
  Serial.println(trigger ? "YES" : "NO");

  delay(300);
}
