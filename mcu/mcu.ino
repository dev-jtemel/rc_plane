const uint8_t TEST_LED = 23;

void setup() {
  pinMode(TEST_LED, OUTPUT);
}

void loop() {
  delay(2000);
  digitalWrite(23, !digitalRead(23));
}
