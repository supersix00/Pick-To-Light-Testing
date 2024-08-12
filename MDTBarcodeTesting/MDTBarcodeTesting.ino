// Adesso NuScan 5200TU
// 0076300342210 (Breakout Box) Barcode testing
int greenLED = 14;
int redLED = 12; 
String barcode = "";
String GTIN = "";
String Batch = "";

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  Serial.begin(115200); // initialize, change to 115200 according to YT video?
}

void loop() {
  if (Serial.available()) { // check if there is serial input
    char num = Serial.read(); // read serial input
    if (isAlphaNumeric(num)) { // if barcode is alphanumeric
      barcode += num;
    }
    if (barcode.length() > 14){
      GTIN = barcode.substring(2, 16); // specific to 2210
      batch = barcode.substring(18); // specific to 2210
    }
    if (num == '\n') {
      Serial.print("barcode: ");
      Serial.println(barcode);
      Serial.print("GTIN: ");
      Serial.println(GTIN);
      Serial.print("Batch: ");
      Serial.println(batch);
      barcode = "";
      GTIN = "";
      batch = "";
    }
  }
}
