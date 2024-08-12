  // Tested with Adesso NuScan 5200TU
// Jonah Kelley

const int greenLED = 14;
const int redLED = 12; 
const int ledPins[] = {15, 16, 17, 18, 19}; // SLOCs, need to make some sort of directory, expand to have multiple LEDs from same pins
int SLOCTest[sizeof(ledPins)];
const int buttonPins[] = {4, 5, 21, 22, 23};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);
int buttonState[numLeds];
int litSLOCs[numLeds];
int pressedLeds = 0;
int litLeds = 0;
int savedLitLeds = 0;
int SLOC = 2999;
int savedSLOC = 2999;
String barcode = "";
String savedBarcode = "";
String GTIN = "";
String Batch = "";

struct KitConfig {
  String serial;
  bool leds[numLeds];
};

KitConfig kitConfigs[] = {
  { "123456", { true, true, true, true, true }},     // { "123456", { true, true, true, true, true }},
  { "55555P", { true, true, false, false, false }}    // { "55555P", { true, false, true, false, true }} 
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
      delay(500);
      digitalWrite(greenLED, LOW); // turn off LED
      digitalWrite(redLED, LOW);
      delay(250);
      
      Serial.println(" ");
      Serial.print("Recieved Kit GTIN: ");
      Serial.println(barcode);
      Serial.println(" ");
      processBarcode(barcode);
      Serial.print("There are ");
      Serial.print(litLeds); // CONSTRAINTS: Assigns digital arduino pin to SLOC so amount of SLOCs is limited, this way might need multiple arduinos with different SLOCs coded
      Serial.print(" components for kit "); // Also assumes that all GTINs come from separate SLOCs
      Serial.print(barcode);
      savedBarcode = barcode;
      barcode = "";
      Serial.println(" in SLOCs: "); // UPDATE: Display quantity of same GTIN in same SLOC for picker to see how many they need to pick, still press button once
      for (int i = 0; i < litLeds; i++){
        SLOC = SLOC + 1;
        SLOCTest[i] = SLOC;
        Serial.print(SLOCTest[i]);
        Serial.println(" - GTIN: _______, Batch: _______");
      } // for loop above is just for testing, need to implement a better way for SLOCs
      SLOC = 2999; 
      savedLitLeds = litLeds;
      litLeds = 0; // reset so it doesnt add to previous scan
    }
  }
  for (int i = 0; i < numLeds; i++){
    if (digitalRead(buttonPins[i]) == LOW){
      if (buttonState[i] == 0 && litSLOCs[i] == 1){  // button hasn't been pressed AND is lit
        digitalWrite(ledPins[i], LOW);
        savedSLOC = savedSLOC + 1;
        SLOCTest[i] = savedSLOC;
        Serial.print("GTIN ____ consumed from SLOC "); // UPDATE TO SHOW COMPONENT GTIN SOMEHOW
        Serial.println(SLOCTest[i]); // UPDATE just goes in order, firgure out how to assign slocs to button presses
        buttonState[i] = 1; 
        litSLOCs[i] = 0; // reset for next scans
        pressedLeds = pressedLeds + 1;
        if (pressedLeds == savedLitLeds){
          delay(250);
          Serial.println(" ");
          Serial.print("Picking complete. All materials for GTIN ");
          Serial.print(savedBarcode);
          Serial.println(" consumed."); 
          Serial.print("=====================================================================================================================================================================================================");
          pressedLeds = 0; 
          savedLitLeds = 0; 
          savedSLOC = 2999; // reset for next scans
        }
      }
    }
  }
  for (int i = 0; i < numLeds; i++){
    buttonState[i] = 0; // reset for next scans
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
    if (kitConfig.leds[i]) {
      digitalWrite(ledPins[i], HIGH); // light LED if coded for that barcode to light
      litSLOCs[i] = 1; // stores which LEDs are lit
      litLeds = litLeds + 1;
    } else {
      digitalWrite(ledPins[i], LOW); // keep LED unlit if not in scope of barcode
    }
  }
}

void turnOffAllLeds() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}