// Adesso NuScan 5200TU
int greenLED = 21;
int redLED = 35;
//const int maxBarcodeLength = 20;
//char barcode[maxBarcodeLength + 1];
//int i = 0;

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  Serial.begin(9600); // initialize
}

void loop() {
  if (Serial.available()) { // check if there is serial input
    char num = Serial.read(); // read serial input
    if (isAlphaNumeric(num)) { // if barcode is alphanumeric
      digitalWrite(greenLED, HIGH); // turn on LED
      digitalWrite(redLED, HIGH);
      delay(500);
      digitalWrite(greenLED, LOW); // turn off LED
      digitalWrite(redLED, LOW);

      //if (i < maxBarcodeLength - 1) {
      //  barcode[i++] = num;
      //  barcode[i] = '\0';
      //}

      Serial.print(num); // print barcode in serial monitor
    }
  }
}