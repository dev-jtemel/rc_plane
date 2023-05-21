const uint8_t TEST_LED = 23;

void setup() {
  Serial.begin(115200);
  pinMode(TEST_LED, OUTPUT);
}

void loop() {
  delay(2000);
  digitalWrite(23, !digitalRead(23));
  Serial.println("Run");
}
