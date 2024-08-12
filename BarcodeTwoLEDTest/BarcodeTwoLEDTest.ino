void setup() {
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  // Initialize other settings
}

void loop() {
  if (Serial.available() > 0) {
    String barcodeData = Serial.readStringUntil('\n');
    barcodeData.trim();
    if (barcodeData.equals("55555P")) {
      // Turn on Green LED
      digitalWrite(13, HIGH);
    } else {
      // Turn on Red LED
      digitalWrite(12, HIGH);
    }
    // Add more conditions for additional barcodes and LEDs
    barcodeData = "";
  }
}
