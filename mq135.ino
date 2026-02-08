const int mq135 = 34;
void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  //set the resolution to 12 bits (0-4095)
  analogReadResolution(12);
  pinMode(mq135,INPUT);
}

void loop() {
  // read the analog / millivolts value for pin 2:
  int analogValue = analogRead(mq135);
  // print out the values you read:
  Serial.printf("ADC analog value = %d\n", analogValue);

  delay(1000);  // delay in between reads for clear read from serial
}