// Bare XIAO on USB only. No other modules or servo power.
void setup() { Serial.begin(115200); }
void loop() {
  Serial.println("Hello from Andrew! This is the Hello lesson.");
  delay(1000); // One job only; later lessons use elapsed-time scheduling.
}
