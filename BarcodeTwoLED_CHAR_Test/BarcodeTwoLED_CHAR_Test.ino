void setup() {
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char receivedChar = Serial.read();
    Serial.print("Received character: ");
    Serial.println(receivedChar);
    if (receivedChar == '1') {
      digitalWrite(13, HIGH); 
    } else if (receivedChar == '2') {
      digitalWrite(12, HIGH); 
    }
    while (Serial.available()) {
      Serial.read(); 
    }
  }
}