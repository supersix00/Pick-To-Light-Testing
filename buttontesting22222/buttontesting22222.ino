// Adesso NuScan 5200TU
int greenLED = 14;
int redLED = 12; 
const int ledPins[] = {15, 16, 17, 18, 19};
const int buttonPins[] = {4, 5, 21, 22, 23};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);
int buttonState = 0;
String barcode = "";

struct KitConfig {
  String serial;
  bool leds[numLeds];
};

KitConfig kitConfigs[] = {
  { "123456", { true, false, true, false, true } }
  //{ "55555P", { false, true, true, true, true } }  
};

const int numKits = sizeof(kitConfigs) / sizeof(kitConfigs[0]);

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  for (int i = 0; i < numLeds; i++){
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  Serial.begin(115200); // initialize, change to 115200 according to YT video?
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

      processBarcode(barcode);
      barcode = ""; 
    }
  }

  for (int i = 0; i < numLeds; i++){
    if (digitalRead(buttonPins[i]) == LOW){
      digitalWrite(ledPins[i], LOW);
    }
  }
}

void processBarcode(const String& barcode) {
  bool kitFound = false;
  for (int i = 0; i < numKits; i++) {
    if (barcode == kitConfigs[i].serial) {
      lightUpKitComponents(kitConfigs[i]);
      kitFound = true;
      break;
    }
  }
  if (kitFound == false) {
    turnOffAllLeds();
    digitalWrite(redLED, HIGH); // turn on red LED for invalid barcode
    delay(1000);
    digitalWrite(redLED, LOW); // turn off red LED
    delay(1000);
    //turnOffAllLeds();
  } else {
    digitalWrite(greenLED, HIGH); // turn on green LED for valid barcode
    delay(1000);
    digitalWrite(greenLED, LOW); // turn off green LED
    delay(1000);
    //delay(4000);
    //turnOffAllLeds();
  }
}

void lightUpKitComponents(const KitConfig& kitConfig) {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], kitConfig.leds[i] ? HIGH : LOW);
  }
}

void turnOffAllLeds() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}