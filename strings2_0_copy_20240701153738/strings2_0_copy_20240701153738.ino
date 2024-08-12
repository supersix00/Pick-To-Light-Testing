// Adesso NuScan 5200TU
int greenLED = 13;
int redLED = 12;
const int maxBarcodeLength = 20;
char barcode[maxBarcodeLength + 1];
int i = 0;

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  Serial.begin(9600); // initialize
}

void loop() {
  if (Serial.available()) { // check if there is serial input
    char num = Serial.read(); // read serial input
    if (isAlphaNumeric(num)) { // if barcode is alphanumeric
      //digitalWrite(greenLED, HIGH); // turn on LED
      //digitalWrite(redLED, HIGH);
      //delay(500);
      //digitalWrite(greenLED, LOW); // turn off LED
      //digitalWrite(redLED, LOW);

      if (i < maxBarcodeLength - 1) {
        barcode[i++] = num;
        barcode[i] = '\0';
      }

       Serial.print(num); // print barcode in serial monitor
      //Serial.print(barcode);

      
      if (strcmp(barcode, "55555P") == 0) { // compares the two strings
        digitalWrite(greenLED, HIGH);
        delay(500);
        digitalWrite(greenLED, LOW);
      }
      else {
        digitalWrite(redLED, HIGH);
        delay(500);
        digitalWrite(redLED, LOW);
      }
      i = 0; // reset
      barcode[0] = '\0'; // reset
      }
    }
  }
