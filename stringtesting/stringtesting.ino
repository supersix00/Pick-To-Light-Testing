// Adesso NuScan 5200TU
int greenLED = 13;
int redLED = 12;

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  Serial.begin(9600); // initialize
}

void loop() {
  if (Serial.available()) { // check if there is serial input
    char key = Serial.read(); // read serial input
    String barcode = String(Serial.read());
    if (isAlphaNumeric(key)) { // if barcode is alphanumeric
      if barcode == '55555P'{
        digitalWrite(greenLED, HIGH); // turn on LED
        delay(500);
        digitalWrite(greenLED, LOW); // turn off LED
      }
      else{
        digitalWrite(redLED, HIGH);
        delay(500);
        digitalWrite(redLED, LOW);
      }
      Serial.print(key); // print barcode in serial monitor
      Serial.print(barcode);
    }
  }
}