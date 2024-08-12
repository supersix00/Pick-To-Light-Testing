// Adesso NuScan 5200TU
int greenLED = 2;
int redLED = 12; 
String barcode = "";

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  Serial.begin(9600); // initialize, change to 115200 according to YT video?
}

void loop() {
  if (Serial.available()) { // check if there is serial input
    char num = Serial.read(); // read serial input
    if (isAlphaNumeric(num)) { // if barcode is alphanumeric
      barcode += num;
    }
    if (num == '\n') {
      digitalWrite(greenLED, HIGH); // turn on LED
      digitalWrite(redLED, HIGH);
      delay(1000);
      digitalWrite(greenLED, LOW); // turn off LED
      digitalWrite(redLED, LOW);
      delay(500);
      
      Serial.print("Recieved: ");
      Serial.println(barcode);
      String correct = String("55555P");
      if (barcode == correct){
        digitalWrite(greenLED, HIGH);
        delay(1000);
        digitalWrite(greenLED, LOW);
      } else{
        digitalWrite(redLED, HIGH);
        delay(1000);
        digitalWrite(redLED, LOW);
      }
      barcode = "";
    }
  }
}
